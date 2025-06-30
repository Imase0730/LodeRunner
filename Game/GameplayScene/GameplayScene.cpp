//--------------------------------------------------------------------------------------
// File: GamePlayScene.cpp
//
//--------------------------------------------------------------------------------------
#include "GamePlayScene.h"
#include "Game/Game.h"
#include "Game/Tile.h"
#include <fstream>
#include <sstream>

// コンストラクタ
GamePlayScene::GamePlayScene(Game* pGame)
	: m_pGame{ pGame }
	, m_updateCounter{ 0 }
	, m_level{ Level::Mode::GamePlay }
	, m_scoreStringRenderer{ POINT{ Tile::TILE_PIXEL_WIDTH * 0 , Level::INFOMATION_Y }, "SCORE" }
	, m_menStringRenderer{ POINT{ Tile::TILE_PIXEL_WIDTH * 13 , Level::INFOMATION_Y }, "MEN" }
	, m_levelStringRenderer{ POINT{ Tile::TILE_PIXEL_WIDTH * 20 , Level::INFOMATION_Y }, "LEVEL" }
	, m_scoreNumberRenderer{ POINT{ 5 * Tile::TILE_PIXEL_WIDTH, Level::INFOMATION_Y }, 7 }
	, m_menNumberRenderer{ POINT{ 16 * Tile::TILE_PIXEL_WIDTH, Level::INFOMATION_Y }, 3 }
	, m_levelNumberRenderer{ POINT{ 25 * Tile::TILE_PIXEL_WIDTH, Level::INFOMATION_Y }, 3 }
	, m_score{ 0 }
	, m_men{ 0 }
	, m_levelId{ 0 }
	, m_player{ this, &m_level }
	, m_pGuard{}
	, m_GuardPattern{ 0, 0, 0 }
	, m_GuardPhase{ 0 }
	, m_GuardNumber{ 0 }
	, m_digBrick{}
	, m_guardResurrectColmun{ 0 }
	, m_startWaitTimer{ 0 }
	, m_clearWaitTimer{ 0 }
	, m_levelClearScore{ 0 }
	, m_playerDeadWaitTimer{ 0 }
	, m_isTestPlay{ false }
	, m_yesNoDialog{}
	, m_dialogType{ DialogType::None }
	, m_saveScore{ 0 }
{
	// ガードを生成
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		m_pGuard[i] = new Guard(this, &m_level);
	}
}

// デストラクタ
GamePlayScene::~GamePlayScene()
{
	// メモリを解放
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		delete m_pGuard[i];
	}
}

// 初期化処理
void GamePlayScene::Initialize()
{
	// テストプレイ中ではない
	m_isTestPlay = false;

	// 登録するスコアの初期化
	m_pGame->InitializeEntryScore();

	// スコアの初期化
	m_score = m_saveScore = 0;
	m_scoreNumberRenderer.SetNumber(m_score);

	// プレイヤーの数の初期化
	m_men = MEN_COUNT;
	m_menNumberRenderer.SetNumber(m_men);

	// レベルの初期化
	m_levelId = 1;
	m_levelNumberRenderer.SetNumber(m_levelId);

	// ゲームスタート時の初期化
	GameInitialize();

	// ワイプオープン
	m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Open);
}

// 更新処理
void GamePlayScene::Update(int keyCondition, int keyTrigger)
{
	// 更新間隔を調整する
	if (++m_updateCounter < UPDATE_INTERVAL) return;
	m_updateCounter = 0;

	static int oldKey = 0;

	keyTrigger = ~oldKey & keyCondition;

	// 次のレベルへの移行処理
	if (WipeToNextLevel()) return;

	// プレイヤー死亡時の待ち時間の処理
	if (WaitPlayerDead()) return;

	// レベルクリア時の待ち時間の処理
	if (WaitLevelClear()) return;

	// Yes/Noダイアログが表示中の処理
	if (HandleDialogState(keyTrigger))
	{
		oldKey = keyCondition;
		return;
	}

	// Qキーでゲームをリトライ
	if (keyTrigger & PAD_INPUT_7)
	{
		m_dialogType = DialogType::Retry;
		m_yesNoDialog.StartDialog("RETRY");
	}

	// F1キーでセーブ
	if (CheckHitKey(KEY_INPUT_F1))
	{
		m_dialogType = DialogType::Save;
		m_yesNoDialog.StartDialog("SAVE");
	}

	// F2キーでロード
	if (CheckHitKey(KEY_INPUT_F2))
	{
		m_dialogType = DialogType::Load;
		m_yesNoDialog.StartDialog("LOAD");
	}

	// ステージの更新
	m_level.Update();

	// プレイヤーの更新
	m_player.Update(keyCondition, keyTrigger);

	// ガードの更新
	UpdateGuards();

	// ----- タイマー関係の処理 ----- //

	// ガードの復活処理
	ResurrectionGuards();

	// ガードの復活位置の更新（列を１足す）
	if (++m_guardResurrectColmun > Level::MAX_GAME_COLMUN) m_guardResurrectColmun = 0;

	// 掘ったブロックを元に戻す処理
	RestoreDigBrick();

	// プレイヤーが生きているなら
	if (m_player.IsAlive())
	{
		// レベルクリアなら得点を１５００点加算する
		if (IsLevelCleared())
		{
			// テストプレイ時
			if (m_isTestPlay)
			{
				// レベルエディタへ
				m_pGame->RequestSceneChange(Game::SceneID::LevelEdit);
			}

			// レベルクリア時の待ち時間を設定
			m_clearWaitTimer = CLEAR_WAIT_FRAME;
			// レベルクリア時の加算するスコアを設定
			m_levelClearScore = LEVEL_CLEAR_SCORE;
			// 効果音を鳴らす（クリア音）
			GetSound().PlaySound(Sound::SoundID::Clear);
		}
	}
	else
	{
		// テストプレイ時
		if (m_isTestPlay)
		{
			// レベルエディタへ
			m_pGame->RequestSceneChange(Game::SceneID::LevelEdit);
		}

		// 残機数を減らす
		m_menNumberRenderer.SetNumber(--m_men);
		// プレイヤー死亡時の待ち時間を設定
		m_playerDeadWaitTimer = PLAYER_DEAD_WAIT_FRAME;
	}

	oldKey = keyCondition;
}

// 描画処理
void GamePlayScene::Render(int ghTileset)
{
	// ステージの描画
	m_level.Render(ghTileset);

	// 復元中のレンガの描画処理
	RenderDigBrick(ghTileset);

	// プレイヤーの描画
	m_player.Render(ghTileset);

	// ガードの描画
	for (int i = 0; i < m_level.GetGuardCount(); i++)
	{
		m_pGuard[i]->Render(ghTileset);
	}

	// 『SCORE』の文字の表示
	m_scoreStringRenderer.Render(ghTileset);

	// 『MEN』の文字の表示
	m_menStringRenderer.Render(ghTileset);

	// 『LEVEL』の文字の表示
	m_levelStringRenderer.Render(ghTileset);

	// スコアの表示
	m_scoreNumberRenderer.Render(ghTileset);

	// 残機数の表示
	m_menNumberRenderer.Render(ghTileset);

	// レベルの表示
	m_levelNumberRenderer.Render(ghTileset);

	// Yes/Noダイアログの描画
	m_yesNoDialog.Render(ghTileset);
}

// 終了処理
void GamePlayScene::Finalize()
{
}

// ゲームスタート時の初期化
void GamePlayScene::GameInitialize()
{
	// ゲームスター時の待ち時間を設定
	m_startWaitTimer = START_WAIT_FRAME;

	// ゲームクリア時の待ち時間を初期化
	m_clearWaitTimer = 0;

	// プレイヤー死亡時の待ち時間を初期化
	m_playerDeadWaitTimer = 0;

	// セーブ用スコアの更新
	m_saveScore = m_score;

	// プレイヤーが生きている
	if (m_player.IsAlive())
	{
		// 残機数を増やす
		m_men++;

		// 次のレベルへ
		m_levelId++;
	}

	// 文字列の更新
	m_scoreNumberRenderer.SetNumber(m_score);
	m_menNumberRenderer.SetNumber(m_men);
	m_levelNumberRenderer.SetNumber(m_levelId);

	// テストプレイ時
	if (m_isTestPlay)
	{
		// テストプレイ用
		Tile::Type (*type)[Level::MAX_GAME_ROW + 1][Level::MAX_GAME_COLMUN + 1] = m_pGame->GetTestPlayData();
		m_level.SetLoadData(*type);
	}
	else
	{
		// レベルデータの読み込み
		if(!m_level.LoadLevel(m_levelId))
		{
			// レベルデータ読み込みエラーのためステージ１に戻る
			m_levelId = 1;
			m_levelNumberRenderer.SetNumber(m_levelId);
			m_level.LoadLevel(m_levelId);
		}
	}

	// レベルの初期化
	m_level.Initialize(Level::Mode::GamePlay);

	// 掘ったレンガを情報の初期化
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		m_digBrick[i].position = POINT{ 0, 0 };
		m_digBrick[i].timer = 0;
	}

	// プレイヤーの初期化
	POINT pos = m_level.GetPlayerPosition();
	m_player.Initialize(POINT{ pos.x, pos.y }
	, POINT{ Tile::TILE_CENTER_X, Tile::TILE_CENTER_Y });

	// ----- ガードの初期化 ----- //

	// ガードの人数に応じて行動可能人数のテーブルを設定する
	for (int i = 0; i < GUARD_PHASE_COUNT; i++)
	{
		m_GuardPattern[i] = GUARD_PATTERNS_LIST[m_level.GetGuardCount() * GUARD_PHASE_COUNT + i];
	}

	// ガードの位置を設定
	for (int i = 0; i < m_level.GetGuardCount(); i++)
	{
		POINT pos = m_level.GetGuardPosition(i);
		m_pGuard[i]->Initialize(POINT{ pos.x, pos.y }
		, POINT{ Tile::TILE_CENTER_X, Tile::TILE_CENTER_Y });
	}
}

// レベルクリアか調べる関数
bool GamePlayScene::IsLevelCleared()
{
	if ( (m_level.GetGoldCount() < 0)								// 隠しハシゴ出現済み
	  && (m_player.GetTilePosition().y == 0)						// 一番上の行？
	  && (m_player.GetAdjustPosition().y == Tile::TILE_CENTER_Y)	// タイル上の位置も中心？
	   )
	{
		return true;
	}
	return false;
}

// ガードの更新処理
void GamePlayScene::UpdateGuards()
{
	// １フレームに行動できるガードの人数分ループ
	for (int i = 0; i < m_GuardPattern[m_GuardPhase]; i++)
	{
		// ガードがアクティブなら更新
		if (m_pGuard[m_GuardNumber]->IsActive()) m_pGuard[m_GuardNumber]->Update();
		// 次の行動するガード番号へ
		m_GuardNumber = (m_GuardNumber + 1) % m_level.GetGuardCount();
	}
	// 次の行動可能人数へ
	m_GuardPhase = (m_GuardPhase + 1) % GUARD_PHASE_COUNT;
}

// ガードの復活処理
void GamePlayScene::ResurrectionGuards()
{
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		int timer = m_pGuard[i]->GetResurrectionTimer();
		if (timer > 0)
		{
			//ステートに大きな値を入れて動かないようにする
			m_pGuard[i]->SetActionStateTimer(0x7f);
			timer--;
			if (timer == 0)
			{
				// 復活場所にガードがいなければ復活する
				POINT pos = m_pGuard[i]->GetTilePosition();
				if (m_level.GetTilePage1(pos.x, pos.y) == Tile::Type::Empty)
				{
					m_level.SetTilePage1(pos.x, pos.y, Tile::Type::Empty);
					m_pGuard[i]->Initialize(m_pGuard[i]->GetTilePosition(), m_pGuard[i]->GetAdjustPosition());
					// 効果音を鳴らす（復活する音）
					GetSound().PlaySound(Sound::SoundID::Resurrection);
				}
			}
			else
			{
				// 復活タイマー減算
				m_pGuard[i]->SetResurrectionTimer(timer);
			}
		}
	}
}

// 復元中のレンガの描画処理
void GamePlayScene::RenderDigBrick(int ghTileset) const
{
	// 復元中のレンガの描画
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		// 復元タイマーが０でない
		if (m_digBrick[i].timer)
		{
			// 掘っているレンガの絵の位置
			POINT pos = m_digBrick[i].position;
			if (m_digBrick[i].timer <= BRICK_ANIME_TIME_FILL02)
			{
				// 復元中のレンガ２
				POINT spritePos = FILL_BRICK_SPRITES[static_cast<int>(FillAnimationState::Fill02)];
				DrawRectGraph(pos.x * Tile::TILE_PIXEL_WIDTH, pos.y * Tile::TILE_PIXEL_HEIGHT
					, Tile::TILE_PIXEL_WIDTH * spritePos.x, Tile::TILE_PIXEL_HEIGHT * spritePos.y
					, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, FALSE);
			}
			else if (m_digBrick[i].timer <= BRICK_ANIME_TIME_FILL01)
			{
				// 復元中のレンガ１
				POINT spritePos = FILL_BRICK_SPRITES[static_cast<int>(FillAnimationState::Fill01)];
				DrawRectGraph(pos.x * Tile::TILE_PIXEL_WIDTH, pos.y * Tile::TILE_PIXEL_HEIGHT
					, Tile::TILE_PIXEL_WIDTH * spritePos.x, Tile::TILE_PIXEL_HEIGHT * spritePos.y
					, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, FALSE);
			}
			else
			{
				// 空白
				POINT spritePos = FILL_BRICK_SPRITES[static_cast<int>(FillAnimationState::EMPTY)];
				DrawRectGraph(pos.x * Tile::TILE_PIXEL_WIDTH, pos.y * Tile::TILE_PIXEL_HEIGHT
					, Tile::TILE_PIXEL_WIDTH * spritePos.x, Tile::TILE_PIXEL_HEIGHT * spritePos.y
					, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, FALSE);
			}
		}
	}
}

// 掘ったレンガを元に戻す処理
void GamePlayScene::RestoreDigBrick()
{
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		// 復元タイマーが０でない
		if (m_digBrick[i].timer != 0)
		{
			// 復元タイマーを減算する
			m_digBrick[i].timer--;

			// レンガに戻る
			if (m_digBrick[i].timer == 0)
			{
				Tile::Type page1 = m_level.GetTilePage1(m_digBrick[i].position.x, m_digBrick[i].position.y);

				// プレイヤーなら
				if (page1 == Tile::Type::Player)
				{
					// プレイヤー死亡
					m_player.SetAlive(false);
					// プレーヤーを非表示
					m_player.SetVisible(false);
				}

				// ガードなら
				if (page1 == Tile::Type::Guard)
				{
					// 死んだガードを見つける
					Guard* Guard = GetGuard(m_digBrick[i].position.x, m_digBrick[i].position.y);
					// 死んだガードが金塊をもっていたら
					if (Guard->GetActionStateTimer())
					{
						// 金塊の数を減らす

						//ステートに大きな値を入れて動かないようにする
						Guard->SetActionStateTimer(0x7f);
					}
					// 復活位置を取得
					POINT pos = GetResurrectPosition(m_guardResurrectColmun);

					// ガードの復活設定
					Guard->Resurrection(pos.x, pos.y);

					// 得点を加算（７５点）
					AddScore(GamePlayScene::GUARD_KILL_SCORE);
				}

				// 金塊なら
				if (page1 == Tile::Type::Gold)
				{
					// 金塊の数を減らす
					m_level.LostGold();
				}

				// レンガに戻す
				m_level.SetTilePage1(m_digBrick[i].position.x, m_digBrick[i].position.y, Tile::Type::Blick);
			}
		}
	}
}

// 次のレベルへの移行処理
bool GamePlayScene::WipeToNextLevel()
{
	// ワイプ動作中なら何もしない
	if (m_pGame->GetIrisWipe()->IsActive()) return true;

	// ワイプが閉じたら
	if (m_pGame->GetIrisWipe()->GetMode() == IrisWipe::Mode::Close)
	{
		// ゲームを初期化
		GameInitialize();

		// ワイプオープン
		m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Open);

		return true;
	}

	// ワイプが終了したら数フレームプレイヤーを点滅させてからゲームを開始する
	if ( (m_pGame->GetIrisWipe()->GetMode() == IrisWipe::Mode::None)
	  && (m_startWaitTimer != 0)
	   )
	{
		// ゲームスタート時のウエイト
		if (m_startWaitTimer > 0) m_startWaitTimer--;
		if (m_startWaitTimer != 0)
		{
			// プレイヤーを点滅
			if ((m_startWaitTimer / 4) % 2)
			{
				m_player.SetVisible(true);
			}
			else
			{
				m_player.SetVisible(false);
			}
			return true;
		}
		else
		{
			// 点滅終了後、プレイヤーを表示(ON)
			m_player.SetVisible(true);
		}
	}

	return false;
}

// レベルクリア時の待ち時間の処理
bool GamePlayScene::WaitLevelClear()
{
	if (m_clearWaitTimer != 0)
	{
		if (m_clearWaitTimer > 0) m_clearWaitTimer--;
		// 次のレベルへ移行するためワイプをクローズする
		if (m_clearWaitTimer == 0)
		{
			m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Close);
		}
		// 100点ずつ足される（合計1500点）
		int score = 100;
		m_levelClearScore -= score;
		if (m_levelClearScore >= 0)
		{
			AddScore(score);
		}
		return true;
	}
	return false;
}

// プレイヤー死亡時の待ち時間の処理
bool GamePlayScene::WaitPlayerDead()
{
	// 画面切り替え時のウエイト
	if (m_playerDeadWaitTimer > 0)
	{
		m_playerDeadWaitTimer--;
		if (m_playerDeadWaitTimer == 0)
		{
			// 残機数が０なら
			if (m_men == 0)
			{
				// スコア登録可能？
				if (m_score > m_pGame->GetScore(Game::SCORE_ENTRY_MAX - 1).score)
				{
					// スコア登録へ
					Game::Score score{ "", m_levelId, m_score };
					m_pGame->SetEntryScore(score);
					m_pGame->RequestSceneChange(Game::SceneID::ScoreRanking);
				}
				else
				{
					// タイトルへ
					m_pGame->RequestSceneChange(Game::SceneID::Title);
				}
			}
			else
			{
				// ワイプクローズ
				m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Close);
			}
		}
		return true;
	}
	return false;
}

// サウンドを取得する関数
const Sound& GamePlayScene::GetSound()
{
	return m_pGame->GetSound();
}

// 得点を加算する関数
void GamePlayScene::AddScore(int score)
{
	m_score += score;
	m_scoreNumberRenderer.SetNumber(m_score);
}

// ガードを取得する関数
Guard* GamePlayScene::GetGuard(int colmun, int row)
{
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		// 指定位置にいるガードへのポインタを返す
		POINT pos = m_pGuard[i]->GetTilePosition();
		if ((pos.x == colmun) && (pos.y == row))
		{
			return m_pGuard[i];
		}
	}
	return nullptr;
}

// 指定位置のレンガを復元する
void GamePlayScene::SetFillBrick(int x, int y)
{
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		// 未使用のワークなら
		if (m_digBrick[i].timer == 0)
		{
			// レンガの復元情報を設定
			m_digBrick[i].position.x = x;
			m_digBrick[i].position.y = y;
			m_digBrick[i].timer = BRICK_FILL_FRAME;
			return;
		}
	}
}

// ガードの復活位置を取得する関数
POINT GamePlayScene::GetResurrectPosition(int colmun)
{
	int row = 1;

	// 上の行から順番に空いている位置を探す
	while (row <= Level::MAX_GAME_ROW)
	{
		for (int i = colmun; i < Level::MAX_GAME_COLMUN; i++)
		{
			// 空白な場所を探す
			if ( (m_level.GetTilePage2(i, row) == Tile::Type::Empty)
			  && (m_level.GetTilePage1(i, row) == Tile::Type::Empty)
			   )
			{
				return POINT{ i, row };
			}
		}
		row++;
		colmun = 0;
	}

	return POINT{ -1, -1 };
}

// デバッグ情報を表示する関数
void GamePlayScene::DisplayDebugInformation(int offsetX, int offsetY) const
{
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 0, GetColor(255, 255, 255)
		, L"<< Keys >>");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 2, GetColor(255, 255, 255)
		, L"       ↑");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 3, GetColor(255, 255, 255)
		, L"MOVE ←　→");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 4, GetColor(255, 255, 255)
		, L"       ↓");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 6, GetColor(255, 255, 255)
		, L"DIG   Z  X");
	
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 8, GetColor(255, 255, 255)
		, L"RETRY  Q");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 10, GetColor(255, 255, 255)
		, L"SAVE  F1");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 12, GetColor(255, 255, 255)
		, L"LOAD  F2");

}

// テストプレイ時の初期化
void GamePlayScene::InitializeTestPlay()
{
	// テストプレイ中
	m_isTestPlay = true;

	// スコアの初期化
	m_score = 0;
	m_scoreNumberRenderer.SetNumber(m_score);

	// プレイヤーの数の初期化
	m_men = 1;
	m_menNumberRenderer.SetNumber(m_men);

	// レベルの初期化
	m_levelId = m_pGame->GetTestPlayLevel();
	m_levelNumberRenderer.SetNumber(m_levelId);

	// ゲームスタート時の初期化
	GameInitialize();
}

// セーブ
bool GamePlayScene::Save()
{
	std::ofstream ofs(SAVE_DATA_FILENAME);

	if (!ofs)
	{
		return false;
	}

	// レベル、残機数、スコア
	ofs << m_levelId << "," << m_men << "," << m_saveScore << "," << std::endl;

	//ファイルを閉じる
	ofs.close();

	return true;
}

// ロード
bool GamePlayScene::Load()
{
	std::ifstream ifs(SAVE_DATA_FILENAME);

	if (!ifs)
	{
		return false;
	}

	// レベル、残機数、スコアの読み込み
	std::string line;
	getline(ifs, line);
	std::istringstream iss(line);
	std::string item;
	getline(iss, item, ',');
	m_levelId = std::stoi(item);
	getline(iss, item, ',');
	m_men = std::stoi(item);
	getline(iss, item, ',');
	m_saveScore = std::stoi(item);

	//ファイルを閉じる
	ifs.close();

	return true;
}

// ダイアログが表示されている時の処理
bool GamePlayScene::HandleDialogState(int keyTrigger)
{
	// YesNoダイアログ表示中？
	if (m_yesNoDialog.IsVisivle())
	{
		// Yesが選択された？
		if ((m_yesNoDialog.Update(keyTrigger) == true)
			&& (m_yesNoDialog.GetYesNo() == YesNoDialog::YesNo::Yes)
			)
		{
			switch (m_dialogType)
			{
			case DialogType::Load:
				if (!Load()) break;
			case DialogType::Retry:
				// プレイヤー死亡時の待ち時間を設定
				m_playerDeadWaitTimer = PLAYER_DEAD_WAIT_FRAME;
				// スコアを戻す
				m_score = m_saveScore;
				// プレイヤーを殺す
				m_player.SetAlive(false);
				break;
			case DialogType::Save:
				Save();
				break;
			default:
				break;
			}
		}
		return true;
	}
	return false;
}

