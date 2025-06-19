//--------------------------------------------------------------------------------------
// File: TitleScene.cpp
//
//--------------------------------------------------------------------------------------
#include "TitleScene.h"
#include "Game/Game.h"
#include "Game/Tile.h"

// コンストラクタ
TitleScene::TitleScene(Game* pGame)
	: m_pGame{ pGame }
	, m_titleStringRenderer{ TITLE_POSITION, "LODE RUNNER" }
	, m_gameStringRenderer{ GAME_POSITION, "GAME" }
	, m_editStringRenderer{ EDIT_POSITION, "EDIT" }
	, m_scoreStringRenderer{ SCORE_POSITION, "SCORE" }
	, m_mode{ Mode::Game }
	, m_blink{ BLINK_INTERVAL }
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
	if (keyTrigger & PAD_INPUT_UP)
	{
		switch (m_mode)
		{
		case TitleScene::Mode::Game:
			m_mode = Mode::Score;
			break;
		case TitleScene::Mode::Edit:
			m_mode = Mode::Game;
			break;
		case TitleScene::Mode::Score:
			m_mode = Mode::Edit;
			break;
		default:
			break;
		}
	}
	if (keyTrigger & PAD_INPUT_DOWN)
	{
		switch (m_mode)
		{
		case TitleScene::Mode::Game:
			m_mode = Mode::Edit;
			break;
		case TitleScene::Mode::Edit:
			m_mode = Mode::Score;
			break;
		case TitleScene::Mode::Score:
			m_mode = Mode::Game;
			break;
		default:
			break;
		}
	}

	// Zキーで決定
	if (keyTrigger & PAD_INPUT_1)
	{
		// 各シーンへ
		switch (m_mode)
		{
		case TitleScene::Mode::Game:
			m_pGame->RequestSceneChange(Game::SceneID::GamePlay);
			break;
		case TitleScene::Mode::Edit:
			m_pGame->RequestSceneChange(Game::SceneID::LevelEdit);
			break;
		case TitleScene::Mode::Score:
			m_pGame->RequestSceneChange(Game::SceneID::ScoreRanking);
			break;
		default:
			break;
		}
	}
}

// 描画処理
void TitleScene::Render(int ghTileset)
{
	// タイトルの表示
	m_titleStringRenderer.Render(ghTileset);

	// GAMEの表示
	m_gameStringRenderer.Render(ghTileset);

	// EDITの表示
	m_editStringRenderer.Render(ghTileset);

	// SCOREの表示
	m_scoreStringRenderer.Render(ghTileset);

	// ----- ここから点滅 ----- //
	int col = static_cast<int>(100.0f + 155.0f * m_blink.GetBlinkRate());
	SetDrawBright(col, col, col);

	// カーソルの表示
	switch (m_mode)
	{
	case TitleScene::Mode::Game:
		DrawRectGraph(GAME_POSITION.x - 18, GAME_POSITION.y + 1
			, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT * 4, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		break;
	case TitleScene::Mode::Edit:
		DrawRectGraph(EDIT_POSITION.x - 18, EDIT_POSITION.y + 1
			, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT * 4, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		break;
	case TitleScene::Mode::Score:
		DrawRectGraph(SCORE_POSITION.x - 18, SCORE_POSITION.y + 1
			, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT * 4, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		break;
	default:
		break;
	}

	// ----- 点滅を止める ----- //
	SetDrawBright(255, 255, 255);
}

// 終了処理
void TitleScene::Finalize()
{
}


