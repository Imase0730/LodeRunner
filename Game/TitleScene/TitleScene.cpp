//--------------------------------------------------------------------------------------
// File: TitleScene.cpp
//
//--------------------------------------------------------------------------------------
#include "TitleScene.h"
#include "Game/Game.h"

// コンストラクタ
TitleScene::TitleScene(Game* pGame)
	: m_pGame{ pGame }
	, m_titleString{ TITLE_POSITION, "LODE RUNNER" }
	, m_gameString{ GAME_POSITION, "GAME" }
	, m_editString{ EDIT_POSITION, "EDIT" }
	, m_mode{ Mode::Game }
	, m_blink{ BLINK_INTEVAL }
{
}

// デストラクタ
TitleScene::~TitleScene()
{
}

// 初期化処理
void TitleScene::Initialize()
{
	// 各変数の初期化
	m_mode = Mode::Game;
}

// 更新処理
void TitleScene::Update(int keyCondition, int keyTrigger)
{
	// 点滅の更新
	m_blink.Update();

	// 上下キーでモードを選択
	if (keyTrigger & PAD_INPUT_UP) m_mode = Mode::Game;
	if (keyTrigger & PAD_INPUT_DOWN) m_mode = Mode::Edit;

	// Zキーで決定
	if (keyTrigger & PAD_INPUT_1)
	{
		// 各シーンへ
		if (m_mode == Mode::Game)
		{
			m_pGame->RequestSceneChange(Game::SceneID::GamePlay);
		}
		else if (m_mode == Mode::Edit)
		{
			m_pGame->RequestSceneChange(Game::SceneID::LevelEdit);
		}
	}
}

// 描画処理
void TitleScene::Render(int ghTileset)
{
	// タイトルの表示
	m_titleString.Render(ghTileset);

	// GAMEの表示
	m_gameString.Render(ghTileset);

	// EDITの表示
	m_editString.Render(ghTileset);

	// ----- ここから点滅 ----- //
	int col = static_cast<int>(100.0f + 155.0f * m_blink.GetBlinkRate());
	SetDrawBright(col, col, col);

	// カーソルの表示
	if (m_mode == Mode::Game)
	{
		DrawRectGraph(GAME_POSITION.x - 18, GAME_POSITION.y + 1
			, Level::TILE_PIXEL_WIDTH * 9, Level::TILE_PIXEL_HEIGHT * 4, Level::TILE_PIXEL_WIDTH, Level::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
	}
	else
	{
		DrawRectGraph(EDIT_POSITION.x - 18, EDIT_POSITION.y + 1
			, Level::TILE_PIXEL_WIDTH * 9, Level::TILE_PIXEL_HEIGHT * 4, Level::TILE_PIXEL_WIDTH, Level::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
	}

	// ----- 点滅を止める ----- //
	SetDrawBright(255, 255, 255);
}

// 終了処理
void TitleScene::Finalize()
{
}


