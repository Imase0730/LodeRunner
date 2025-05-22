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
}

// 終了処理
void GamePlayScene::Finalize()
{
}


