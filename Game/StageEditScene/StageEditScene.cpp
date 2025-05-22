//--------------------------------------------------------------------------------------
// File: StageEditScene.cpp
//
//--------------------------------------------------------------------------------------
#include "StageEditScene.h"
#include "Game/Game.h"

// コンストラクタ
StageEditScene::StageEditScene(Game* pGame)
	: m_pGame{ pGame }
	, m_stage{ Stage::Mode::StageEdit }
	, m_ghTileset{}
	, m_cursorEdit{ 0, 0 }
	, m_selectTile{ Tile::TileType::Blick }
	, m_blink{ BLINK_INTEVAL }
	, m_level{ 1 }
	, m_levelNumber{ POINT{ 25 * Tile::TILE_WIDTH, Screen::GAME_HEIGHT - Tile::TILE_HEIGHT }, 3 }
	, m_mode{ Mode::Tile }
{
}

// デストラクタ
StageEditScene::~StageEditScene()
{
}

// 初期化処理
void StageEditScene::Initialize(int ghTileset)
{
	m_ghTileset = ghTileset;
	// レベルの設定
	m_levelNumber.SetNumber(m_level);
}

// 更新処理
void StageEditScene::Update(int keyCondition, int keyTrigger)
{
	static int elapseFrame = 0;
	int keyRepeat = 0;

	m_blink.Update();

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

	// スペースキーでモード変更
	if (keyTrigger & PAD_INPUT_10)
	{
		switch (m_mode)
		{
		case Mode::Tile:
			m_mode = Mode::Save;
			break;
		case Mode::Save:
			m_mode = Mode::Load;
			break;
		case Mode::Load:
			m_mode = Mode::Tile;
			break;
		default:
			break;
		}
	}

	// 選択中のモードにより処理を分ける
	switch (m_mode)
	{
	case Mode::Tile:
		SelectTile(keyCondition, keyRepeat);
		break;
	case Mode::Save:
		Save(keyTrigger, keyRepeat);
		break;
	case Mode::Load:
		Load(keyTrigger, keyRepeat);
		break;
	default:
		break;
	}

}

// 描画処理
void StageEditScene::Render()
{
	// ステージの描画
	m_stage.Render(m_ghTileset);

	// 点滅
	int col = static_cast<int>(100.0f + 155.0f * m_blink.GetBlinkRate());
	SetDrawBright(col, col, col);

	switch (m_mode)
	{
	case Mode::Tile:
		// カーソルの描画（上部）
		DrawRectGraph(m_cursorEdit.x * Tile::TILE_WIDTH, m_cursorEdit.y * Tile::TILE_HEIGHT
			, Tile::TILE_WIDTH * 9, Tile::TILE_HEIGHT * 1
			, Tile::TILE_WIDTH, Tile::TILE_HEIGHT, m_ghTileset, TRUE);

		// カーソルの描画（下部）
		DrawRectGraph(Tile::TILE_WIDTH * (static_cast<int>(m_selectTile) - 1), Screen::GAME_HEIGHT - Tile::TILE_HEIGHT
			, Tile::TILE_WIDTH * 9, Tile::TILE_HEIGHT * 1
			, Tile::TILE_WIDTH, Tile::TILE_HEIGHT, m_ghTileset, TRUE);

		// LEVELの点滅を止める
		SetDrawBright(255, 255, 255);
		break;
	case Mode::Save:
		// SAVEの描画
		DrawRectGraph(Tile::TILE_WIDTH * 10, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5
			, Tile::TILE_WIDTH * 0, Tile::TILE_HEIGHT * 3
			, Tile::TILE_WIDTH * 4, Tile::TILE_HEIGHT, m_ghTileset, TRUE);
		break;
	case Mode::Load:
		// LOADの描画
		DrawRectGraph(Tile::TILE_WIDTH * 15, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5
			, Tile::TILE_WIDTH * 4, Tile::TILE_HEIGHT * 3
			, Tile::TILE_WIDTH * 4, Tile::TILE_HEIGHT, m_ghTileset, TRUE);
		break;
	default:
		break;
	}

	// レベルの表示
	m_levelNumber.Render(m_ghTileset);

	// 点滅を止める
	SetDrawBright(255, 255, 255);
}

// 終了処理
void StageEditScene::Finalize()
{
}

void StageEditScene::SelectTile(int keyCondition, int keyRepeat)
{
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
	// AキーとDキーで選択タイルの変更
	if (keyRepeat & PAD_INPUT_4)
	{
		if (m_selectTile != Tile::TileType::Blick)
		{
			m_selectTile = static_cast<Tile::TileType>((static_cast<int>(m_selectTile) - 1));
		}
	}
	if (keyRepeat & PAD_INPUT_6)
	{
		if (m_selectTile != Tile::TileType::Player)
		{
			m_selectTile = static_cast<Tile::TileType>((static_cast<int>(m_selectTile) + 1));
		}
	}
	// Sキーでカーソル位置のタイルを選択
	if (keyCondition & PAD_INPUT_5)
	{
		Tile::TileType type = m_stage.GetTileType(m_cursorEdit.x, m_cursorEdit.y);
		if (type != Tile::TileType::None) m_selectTile = type;
	}
}

// セーブ
void StageEditScene::Save(int keyTrigger, int keyRepeat)
{
	if (keyTrigger & PAD_INPUT_1)
	{
		m_stage.SaveLevel(m_level);
	}
	if (keyRepeat & PAD_INPUT_UP)
	{
		if (m_level < STAGE_MAX) m_level++;
	}
	if (keyRepeat & PAD_INPUT_DOWN)
	{
		if (m_level > 1) m_level--;
	}
	// レベルの設定
	m_levelNumber.SetNumber(m_level);
}

// ロード
void StageEditScene::Load(int keyTrigger, int keyRepeat)
{
	if (keyTrigger & PAD_INPUT_1)
	{
		m_stage.LoadLevel(m_level);
	}
	if (keyRepeat & PAD_INPUT_UP)
	{
		if (m_level < STAGE_MAX) m_level++;
	}
	if (keyRepeat & PAD_INPUT_DOWN)
	{
		if (m_level > 1) m_level--;
	}
	// レベルの設定
	m_levelNumber.SetNumber(m_level);
}


