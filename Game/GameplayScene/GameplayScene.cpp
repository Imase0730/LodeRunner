//--------------------------------------------------------------------------------------
// File: GameplayScene.cpp
//
//--------------------------------------------------------------------------------------
#include "GameplayScene.h"
#include "Game/Game.h"

// コンストラクタ
GameplayScene::GameplayScene(Game* pGame)
	: m_pGame{ pGame }
	, m_stage{ this }
	, m_mode{ Mode::Edit }
	, m_ghTileset{}
	, m_cursorEdit{ 0, 0 }
	, m_selectTile{ Tile::TileType::Blick }
	, m_blink{ BLINK_INTEVAL }
{
	// 絵のロード
	m_ghTileset = LoadGraph(L"Resources/Textures/tileset.png");
}

// デストラクタ
GameplayScene::~GameplayScene()
{
	// 絵のデータをメモリから削除
	DeleteGraph(m_ghTileset);
}

// 初期化処理
void GameplayScene::Initialize()
{

}

// 更新処理
void GameplayScene::Update(int keyCondition, int keyTrigger)
{
	static int elapseFrame = 0;
	int keyRepeat = 0;

	m_blink.Update();

	if (m_mode == Mode::Edit)
	{
		// 簡易キーリピート
		if (keyCondition)
		{
			elapseFrame++;
			if (elapseFrame >= KEY_REPEAT_INTEVAL)
			{
				elapseFrame = 0;
				keyRepeat = keyCondition;
			}
		}

		// 上部カーソルの移動
		if (keyRepeat & PAD_INPUT_UP)    m_cursorEdit.y--;
		if (keyRepeat & PAD_INPUT_DOWN)  m_cursorEdit.y++;
		if (keyRepeat & PAD_INPUT_LEFT)  m_cursorEdit.x--;
		if (keyRepeat & PAD_INPUT_RIGHT) m_cursorEdit.x++;

		// 上部カーソルの画面外チェック
		if (m_cursorEdit.x < 0) m_cursorEdit.x = 0;
		if (m_cursorEdit.x > Stage::STAGE_WIDTH - 1) m_cursorEdit.x = Stage::STAGE_WIDTH - 1;
		if (m_cursorEdit.y < 0) m_cursorEdit.y = 0;
		if (m_cursorEdit.y > Stage::STAGE_HEIGHT - 1) m_cursorEdit.y = Stage::STAGE_HEIGHT - 1;

		// Zキーでカーソル位置に指定タイルを置く
		if (keyCondition & PAD_INPUT_1)
		{
			m_stage.SetTileType(m_cursorEdit.x, m_cursorEdit.y, m_selectTile);
		}
		// Xキーでカーソル位置のタイルを消す
		if (keyCondition & PAD_INPUT_2)
		{
			m_stage.SetTileType(m_cursorEdit.x, m_cursorEdit.y, Tile::TileType::None);
		}
		// Cキーで選択タイルの変更
		if (keyRepeat & PAD_INPUT_3)
		{
			m_selectTile = static_cast<Tile::TileType>((static_cast<int>(m_selectTile) + 1));
			if (m_selectTile == Tile::TileType::TileTypeMax) m_selectTile = Tile::TileType::Blick;
		}
	}
}

// 描画処理
void GameplayScene::Render()
{
	// ステージの描画
	m_stage.Render(m_ghTileset);

	if (m_mode == Mode::Edit)
	{
		int col = static_cast<int>(100.0f + 155.0f * m_blink.GetBlinkRate());
		SetDrawBright(col, col, col);

		// カーソルの描画
		DrawRectGraph(m_cursorEdit.x * Tile::TILE_WIDTH, m_cursorEdit.y * Tile::TILE_HEIGHT
			, Tile::TILE_WIDTH * 9, Tile::TILE_HEIGHT * 1
			, Tile::TILE_WIDTH, Tile::TILE_HEIGHT, m_ghTileset, TRUE);

		POINT pos[static_cast<int>(Tile::TileType::TileTypeMax)] =
		{
			Tile::TILE_WIDTH * 0, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 0, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 1, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 2, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 3, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 4, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 5, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 6, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 7, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 8, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
		};
		DrawRectGraph(pos[static_cast<int>(m_selectTile)].x, pos[static_cast<int>(m_selectTile)].y
			, Tile::TILE_WIDTH * 9, Tile::TILE_HEIGHT * 1
			, Tile::TILE_WIDTH, Tile::TILE_HEIGHT, m_ghTileset, TRUE);
		
		SetDrawBright(255, 255, 255);
	}
}

// 終了処理
void GameplayScene::Finalize()
{
}


