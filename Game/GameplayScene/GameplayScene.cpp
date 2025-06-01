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
	, m_scoreString{ POINT{ Tile::TILE_WIDTH * 0 , Game::INFOMATION_Y }, "SCORE" }
	, m_menString{ POINT{ Tile::TILE_WIDTH * 13 , Game::INFOMATION_Y }, "MEN" }
	, m_levelString{ POINT{ Tile::TILE_WIDTH * 20 , Game::INFOMATION_Y }, "LEVEL" }
	, m_scoreNumber{ POINT{ 5 * Tile::TILE_WIDTH, Game::INFOMATION_Y }, 7 }
	, m_menNumber{ POINT{ 16 * Tile::TILE_WIDTH, Game::INFOMATION_Y }, 3 }
	, m_levelNumber{ POINT{ 25 * Tile::TILE_WIDTH, Game::INFOMATION_Y }, 3 }
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

	// ステージのロード
	m_stage.LoadLevel(m_level, Stage::Mode::GamePlay);

	// プレイヤーの初期化
	POINT pos = m_stage.GetPlayerPosition();
	m_player.Initialize(POINT{ pos.x * Tile::TILE_WIDTH + Tile::CENTER_OFFSET_X
		, pos.y * Tile::TILE_HEIGHT + Tile::CENTER_OFFSET_Y });

	// アイリスワイプオープン
	m_pGame->GetIrisWipe()->Initialize(IrisWipe::Mode::Open);
	m_pGame->GetIrisWipe()->Start();
}

// 更新処理
void GamePlayScene::Update(int keyCondition, int keyTrigger)
{
	// 更新間隔を調整する
	if (++m_updateCounter < UPDATE_INTERVAL) return;
	m_updateCounter = 0;

	// プレイヤーの更新
	m_player.Update(keyCondition, keyTrigger, &m_stage);
}

// 描画処理
void GamePlayScene::Render(int ghTileset)
{
	// ステージの描画
	m_stage.Render(ghTileset);

	// プレイヤーの描画
	m_player.Render(ghTileset);

	// スコアの表示
	m_scoreString.Render(ghTileset);

	// 残機数の表示
	m_menString.Render(ghTileset);

	// レベルの表示
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


