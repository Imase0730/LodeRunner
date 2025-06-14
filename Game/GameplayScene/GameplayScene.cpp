//--------------------------------------------------------------------------------------
// File: GamePlayScene.cpp
//
//--------------------------------------------------------------------------------------
#include "GamePlayScene.h"
#include "Game/Game.h"
#include "Game/Tile.h"

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
	, m_pGurad{}
	, m_guradPattern{ 0, 0, 0 }
	, m_guradPhase{ 0 }
	, m_guradNumber{ 0 }
	, m_digBrick{}
	, m_guardResurrectColmun{ 0 }
	, m_startWaitTimer{ 0 }
	, m_clearWaitTimer{ 0 }
	, m_levelClearScore{ 0 }
{
	// ガードを生成
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		m_pGurad[i] = new Gurad(this, &m_level);
	}
}

// デストラクタ
GamePlayScene::~GamePlayScene()
{
	// メモリを解放
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		delete m_pGurad[i];
	}
}

// 初期化処理
void GamePlayScene::Initialize()
{
	// スコアの初期化
	m_score = 0;
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

	// レベルクリア時の待ち時間の処理
	if (WaitLevelClear()) return;

	// ステージの更新
	m_level.Update();

	// プレイヤーの更新
	m_player.Update(keyCondition, keyTrigger);

	// ガードの更新
	UpdateGurads();

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
			// レベルクリア時の待ち時間を設定
			m_clearWaitTimer = CLEAR_WAIT_FRAME;
			// レベルクリア時の加算するスコアを設定
			m_levelClearScore = LEVEL_CLEAR_SCORE;
		}
	}
	else
	{
		// 残機数を減らす
		m_menNumberRenderer.SetNumber(--m_men);

		// 残機数が０なら
		if (m_men == 0)
		{
			// タイトルへ
			m_pGame->RequestSceneChange(Game::SceneID::Title);
		}
		else
		{
			// ワイプクローズ
			m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Close);
		}
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
		m_pGurad[i]->Render(ghTileset);
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

	// プレイヤーが生きている
	if (m_player.IsAlive())
	{
		// 残機数を増やす
		m_menNumberRenderer.SetNumber(++m_men);

		// 次のレベルへ
		m_levelNumberRenderer.SetNumber(++m_levelId);
	}

	// ステージのロード
	m_level.Initialize(m_levelId, Level::Mode::GamePlay);

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
		m_guradPattern[i] = GUARD_PATTERNS_LIST[m_level.GetGuardCount() * GUARD_PHASE_COUNT + i];
	}

	// ガードの位置を設定
	for (int i = 0; i < m_level.GetGuardCount(); i++)
	{
		POINT pos = m_level.GetGuardPosition(i);
		m_pGurad[i]->Initialize(POINT{ pos.x, pos.y }
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
void GamePlayScene::UpdateGurads()
{
	// １フレームに行動できるガードの人数分ループ
	for (int i = 0; i < m_guradPattern[m_guradPhase]; i++)
	{
		// ガードがアクティブなら更新
		if (m_pGurad[m_guradNumber]->IsActive()) m_pGurad[m_guradNumber]->Update();
		// 次の行動するガード番号へ
		m_guradNumber = (m_guradNumber + 1) % m_level.GetGuardCount();
	}
	// 次の行動可能人数へ
	m_guradPhase = (m_guradPhase + 1) % GUARD_PHASE_COUNT;
}

// ガードの復活処理
void GamePlayScene::ResurrectionGuards()
{
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		int timer = m_pGurad[i]->GetResurrectionTimer();
		if (timer > 0)
		{
			// 金塊保持タイマーに大きな値を入れて動かないようにする
			m_pGurad[i]->SetGoldTimer(0x7f);
			timer--;
			if (timer == 0)
			{
				// 復活場所にガードがいなければ復活する
				POINT pos = m_pGurad[i]->GetTilePosition();
				if (m_level.GetTilePage1(pos.x, pos.y) == Tile::Type::Empty)
				{
					m_level.SetTilePage1(pos.x, pos.y, Tile::Type::Empty);
					m_pGurad[i]->Initialize(m_pGurad[i]->GetTilePosition(), m_pGurad[i]->GetAdjustPosition());
				}
			}
			else
			{
				// 復活タイマー減算
				m_pGurad[i]->SetResurrectionTimer(timer);
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
					Gurad* gurad = GetGurad(m_digBrick[i].position.x, m_digBrick[i].position.y);
					// 死んだガードが金塊をもっていたら
					if (gurad->GetGoldTimer())
					{
						// 金塊の数を減らす

						// 金塊保持タイマーに大きな値を入れて動かないようにする
						gurad->SetGoldTimer(0x7f);
					}
					// 復活位置を取得
					POINT pos = GetResurrectPosition(m_guardResurrectColmun);

					// ガードの復活設定
					gurad->Resurrection(pos.x, pos.y);

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

	// ワイプが終了したら
	if (m_pGame->GetIrisWipe()->GetMode() == IrisWipe::Mode::None)
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

// 得点を加算する関数
void GamePlayScene::AddScore(int score)
{
	m_score += score;
	m_scoreNumberRenderer.SetNumber(m_score);
}

// ガードを取得する関数
Gurad* GamePlayScene::GetGurad(int colmun, int row)
{
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		// 指定位置にいるガードへのポインタを返す
		POINT pos = m_pGurad[i]->GetTilePosition();
		if ((pos.x == colmun) && (pos.y == row))
		{
			return m_pGurad[i];
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

