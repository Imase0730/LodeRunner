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
	, m_stage{ Stage::Mode::GamePlay }
	, m_scoreString{ POINT{ Tile::TILE_PIXEL_WIDTH * 0 , Game::INFOMATION_Y }, "SCORE" }
	, m_menString{ POINT{ Tile::TILE_PIXEL_WIDTH * 13 , Game::INFOMATION_Y }, "MEN" }
	, m_levelString{ POINT{ Tile::TILE_PIXEL_WIDTH * 20 , Game::INFOMATION_Y }, "LEVEL" }
	, m_scoreNumber{ POINT{ 5 * Tile::TILE_PIXEL_WIDTH, Game::INFOMATION_Y }, 7 }
	, m_menNumber{ POINT{ 16 * Tile::TILE_PIXEL_WIDTH, Game::INFOMATION_Y }, 3 }
	, m_levelNumber{ POINT{ 25 * Tile::TILE_PIXEL_WIDTH, Game::INFOMATION_Y }, 3 }
	, m_score{ 0 }
	, m_men{ 0 }
	, m_level{ 0 }
	, m_player{}
{
}

// デストラクタ
GamePlayScene::~GamePlayScene()
{
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
	m_level = 1;
	m_levelNumber.SetNumber(m_level);

	// ゲームスタート時の初期化
	GameInitialize();
}

// 更新処理
void GamePlayScene::Update(int keyCondition, int keyTrigger)
{
	// 更新間隔を調整する
	if (++m_updateCounter < UPDATE_INTERVAL) return;
	m_updateCounter = 0;

	static int oldKey = 0;

	// ワイプ動作中
	if (m_pGame->GetIrisWipe()->IsActive()) return;

	// ワイプが閉じたら
	if (m_pGame->GetIrisWipe()->GetMode() == IrisWipe::Mode::Close)
	{
		// ゲームを初期化してワイプオープン
		GameInitialize();
	}

	// ステージの更新
	m_stage.Update();

	// プレイヤーの更新
	m_player.Update(keyCondition, ~oldKey & keyCondition, &m_stage);

	// プレイヤーが死んだら
	if (!m_player.IsAlive())
	{
		// 残機数を減らす
		m_men--;
		m_menNumber.SetNumber(m_men);

		// 残機数が０なら
		if (m_men == 0)
		{
			// タイトルへ
			m_pGame->RequestSceneChange(Game::SceneID::Title);
		}
		else
		{
			// ワイプクローズしてやり直し
			m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Close);
		}
	}

	oldKey = keyCondition;
}

// 描画処理
void GamePlayScene::Render(int ghTileset)
{
	// ステージの描画
	m_stage.Render(ghTileset);

	// プレイヤーの描画
	m_player.Render(ghTileset);

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
	// ステージのロード
	m_stage.Initialize(m_level, Stage::Mode::GamePlay);

	// プレイヤーの初期化
	POINT pos = m_stage.GetPlayerPosition();
	m_player.Initialize(POINT{ pos.x, pos.y }
	, POINT{ Tile::TILE_CENTER_X, Tile::TILE_CENTER_Y });

	// ワイプオープン
	m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Open);
}


