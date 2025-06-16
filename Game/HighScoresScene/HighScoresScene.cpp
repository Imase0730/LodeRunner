//--------------------------------------------------------------------------------------
// File: HighScoresScene.cpp
//
//--------------------------------------------------------------------------------------
#include "HighScoresScene.h"
#include "Game/Game.h"

// コンストラクタ
HighScoresScene::HighScoresScene(Game* pGame)
	: m_pGame{ pGame }
	, m_mode{ Mode::Game }
	, m_stringRenderer{ POINT{ 0, 0 },"" }
	, m_numberRenderer{ POINT{ 0, 0 }, 2, false }
{
}

// デストラクタ
HighScoresScene::~HighScoresScene()
{
}

// 初期化処理
void HighScoresScene::Initialize()
{

}

// 更新処理
void HighScoresScene::Update(int keyCondition, int keyTrigger)
{
	// Qキーでタイトルへ
	if (keyTrigger & PAD_INPUT_7)
	{
		m_pGame->RequestSceneChange(Game::SceneID::Title);
		return;
	}
}

// 描画処理
void HighScoresScene::Render(int ghTileset)
{
	// タイトル
	m_stringRenderer.SetString("LODE RUNNER HIGH SCORES");
	m_stringRenderer.SetPosition(POINT{ 4 * Tile::TILE_PIXEL_WIDTH, 0 });
	m_stringRenderer.Render(ghTileset);

	m_stringRenderer.SetString("INITIALS LEVEL  SCORE");
	m_stringRenderer.SetPosition(POINT{ 4 * Tile::TILE_PIXEL_WIDTH, 3 * Tile::TILE_PIXEL_HEIGHT });
	m_stringRenderer.Render(ghTileset);

	// 横の線
	for (int i = 0; i < 23; i++)
	{
		if ((i != 8) && (i != 14))
		{
			DrawRectGraph(4 * Tile::TILE_PIXEL_WIDTH + i * Tile::TILE_PIXEL_WIDTH, 4 * Tile::TILE_PIXEL_HEIGHT
				, Tile::TILE_PIXEL_WIDTH * 7, Tile::TILE_PIXEL_HEIGHT * 3
				, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT
				, ghTileset, TRUE);
		}
	}

	// スコア
	for (int i = 0; i < 10; i++)
	{
		// 順位
		m_numberRenderer.SetNumber(i + 1);
		m_numberRenderer.SetPosition(POINT{ 0, (5 + i) * Tile::TILE_PIXEL_HEIGHT });
		m_numberRenderer.Render(ghTileset);

		// ドット
		DrawRectGraph(2 * Tile::TILE_PIXEL_WIDTH, (5 + i) * Tile::TILE_PIXEL_HEIGHT
			, Tile::TILE_PIXEL_WIDTH * 6, Tile::TILE_PIXEL_HEIGHT * 3
			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT
			, ghTileset, TRUE);
		
	}
}

// 終了処理
void HighScoresScene::Finalize()
{
}


