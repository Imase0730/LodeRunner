//--------------------------------------------------------------------------------------
// File: GamePlayScene.cpp
//
//--------------------------------------------------------------------------------------
#include "GamePlayScene.h"
#include "Game/Game.h"

// コンストラクタ
GamePlayScene::GamePlayScene(Game* pGame)
	: m_pGame{ pGame }
	, m_updateCounter{ 0 }
	, m_level{ Level::Mode::GamePlay }
	, m_scoreString{ POINT{ Level::TILE_PIXEL_WIDTH * 0 , Game::INFOMATION_Y }, "SCORE" }
	, m_menString{ POINT{ Level::TILE_PIXEL_WIDTH * 13 , Game::INFOMATION_Y }, "MEN" }
	, m_levelString{ POINT{ Level::TILE_PIXEL_WIDTH * 20 , Game::INFOMATION_Y }, "LEVEL" }
	, m_scoreNumber{ POINT{ 5 * Level::TILE_PIXEL_WIDTH, Game::INFOMATION_Y }, 7 }
	, m_menNumber{ POINT{ 16 * Level::TILE_PIXEL_WIDTH, Game::INFOMATION_Y }, 3 }
	, m_levelNumber{ POINT{ 25 * Level::TILE_PIXEL_WIDTH, Game::INFOMATION_Y }, 3 }
	, m_score{ 0 }
	, m_men{ 0 }
	, m_levelId{ 0 }
	, m_player{ this, &m_level }
	, m_pGurad{}
	, m_guradPattern{ 0, 0, 0 }
	, m_guradPhase{ 0 }
	, m_guradNumber{ 0 }
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
	m_scoreNumber.SetNumber(m_score);

	// プレイヤーの数の初期化
	m_men = MEN_COUNT;
	m_menNumber.SetNumber(m_men);

	// レベルの初期化
	m_levelId = 1;
	m_levelNumber.SetNumber(m_levelId);

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

	// ワイプ動作中なら何もしない
	if (m_pGame->GetIrisWipe()->IsActive()) return;

	// ワイプが閉じたら
	if (m_pGame->GetIrisWipe()->GetMode() == IrisWipe::Mode::Close)
	{
		// ゲームを初期化
		GameInitialize();
		// ワイプオープン
		m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Open);
	}

	// ステージの更新
	m_level.Update();

	// プレイヤーの更新
	m_player.Update(keyCondition, keyTrigger);

	// ガードの更新
	GuradsUpdate();

	// プレイヤーが生きているなら
	if (m_player.IsAlive())
	{
		// ステージクリアならワイプを閉じる
		if (IsLevelCleared()) m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Close);
	}
	else
	{
		// 残機数を減らす
		m_menNumber.SetNumber(--m_men);

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

	// プレイヤーの描画
	m_player.Render(ghTileset);

	// ガードの描画
	for (int i = 0; i < m_level.GetGuardCount(); i++)
	{
		m_pGurad[i]->Render(ghTileset);
	}

	// 『SCORE』の文字の表示
	m_scoreString.Render(ghTileset);

	// 『MEN』の文字の表示
	m_menString.Render(ghTileset);

	// 『LEVEL』の文字の表示
	m_levelString.Render(ghTileset);

	// スコアの表示
	m_scoreNumber.Render(ghTileset);

	// 残機数の表示
	m_menNumber.Render(ghTileset);

	// レベルの表示
	m_levelNumber.Render(ghTileset);
}

// 終了処理
void GamePlayScene::Finalize()
{
}

// ゲームスタート時の初期化
void GamePlayScene::GameInitialize()
{
	// プレイヤーが生きている
	if (m_player.IsAlive())
	{
		// 次のレベルへ
		m_levelNumber.SetNumber(++m_levelId);
	}

	// ステージのロード
	m_level.Initialize(m_levelId, Level::Mode::GamePlay);

	// プレイヤーの初期化
	POINT pos = m_level.GetPlayerPosition();
	m_player.Initialize(POINT{ pos.x, pos.y }
	, POINT{ Level::TILE_CENTER_X, Level::TILE_CENTER_Y });

	//// ----- ガードの初期化 ----- //

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
		, POINT{ Level::TILE_CENTER_X, Level::TILE_CENTER_Y });
	}
}

// レベルクリアか調べる関数
bool GamePlayScene::IsLevelCleared()
{
	if ( (m_level.GetGoldCount() < 0)								// 隠しハシゴ出現済み
	  && (m_player.GetTilePosition().y == 0)						// 一番上の行？
	  && (m_player.GetAdjustPosition().y == Level::TILE_CENTER_Y)	// タイル上の位置も中心？
	   )
	{
		return true;
	}
	return false;
}

// ガードの更新処理
void GamePlayScene::GuradsUpdate()
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

// 得点を加算する関数
void GamePlayScene::AddScore(int score)
{
	m_score += score;
	m_scoreNumber.SetNumber(m_score);
}


