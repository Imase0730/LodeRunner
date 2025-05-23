//--------------------------------------------------------------------------------------
// File: GamePlayScene.cpp
//
//--------------------------------------------------------------------------------------
#include "GamePlayScene.h"
#include "Game/Game.h"

// コンストラクタ
GamePlayScene::GamePlayScene(Game* pGame)
	: m_pGame{ pGame }
	, m_stage{ Stage::Mode::GamePlay }
	, m_ghTileset{ -1 }
	, m_scoreString{ POINT{ Tile::TILE_WIDTH * 0 , Game::INFOMATION_Y }, "SCORE" }
	, m_menString{ POINT{ Tile::TILE_WIDTH * 13 , Game::INFOMATION_Y }, "MEN" }
	, m_levelString{ POINT{ Tile::TILE_WIDTH * 20 , Game::INFOMATION_Y }, "LEVEL" }
	, m_scoreNumber{ POINT{ 5 * Tile::TILE_WIDTH, Screen::GAME_HEIGHT - Tile::TILE_HEIGHT }, 7 }
	, m_menNumber{ POINT{ 16 * Tile::TILE_WIDTH, Screen::GAME_HEIGHT - Tile::TILE_HEIGHT }, 3 }
	, m_levelNumber{ POINT{ 25 * Tile::TILE_WIDTH, Screen::GAME_HEIGHT - Tile::TILE_HEIGHT }, 3 }
{
}

// デストラクタ
GamePlayScene::~GamePlayScene()
{
}

// 初期化処理
void GamePlayScene::Initialize(int ghTileset)
{
	m_ghTileset = ghTileset;
}

// 更新処理
void GamePlayScene::Update(int keyCondition, int keyTrigger)
{
}

// 描画処理
void GamePlayScene::Render()
{
	// ステージの描画
	m_stage.Render(m_ghTileset);

	// スコアの表示
	m_scoreString.Render(m_ghTileset);

	// 残機数の表示
	m_menString.Render(m_ghTileset);

	// レベルの表示
	m_levelString.Render(m_ghTileset);

	// スコアの表示
	m_scoreNumber.Render(m_ghTileset);

	// 残機数の表示
	m_menNumber.Render(m_ghTileset);

	// レベルの表示
	m_levelNumber.Render(m_ghTileset);
}

// 終了処理
void GamePlayScene::Finalize()
{
}


