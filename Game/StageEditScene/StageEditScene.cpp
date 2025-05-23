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
	, m_ghTileset{ -1 }
	, m_cursorEdit{ 0, 0 }
	, m_selectTile{ Tile::TileType::Blick }
	, m_blink{ BLINK_INTEVAL }
	, m_level{ 1 }
	, m_levelNumber{ POINT{ 25 * Tile::TILE_WIDTH, Screen::GAME_HEIGHT - Tile::TILE_HEIGHT }, 3 }
	, m_mode{ Mode::SelectTile }
{
}

// デストラクタ
StageEditScene::~StageEditScene()
{
}

// 初期化処理
void StageEditScene::Initialize(int ghTileset)
{
	// タイルセットのグラフィックハンドル
	m_ghTileset = ghTileset;
	// レベルの設定
	m_levelNumber.SetNumber(m_level);
}

// 更新処理
void StageEditScene::Update(int keyCondition, int keyTrigger)
{
	// 簡易キーリピート
	int keyRepeat = KeyRepeat(keyCondition);

	// 点滅の更新
	m_blink.Update();

	// Qキーでモード変更
	if (keyTrigger & PAD_INPUT_7)
	{
		switch (m_mode)
		{
		case Mode::SelectTile:
			m_mode = Mode::Save;
			break;
		case Mode::Save:
			m_mode = Mode::Load;
			break;
		case Mode::Load:
			m_mode = Mode::SelectTile;
			break;
		default:
			break;
		}
	}

	// 選択中のモードにより処理を分ける
	switch (m_mode)
	{
	case Mode::SelectTile:
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
	case Mode::SelectTile:
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

// 簡易キーリピート
int StageEditScene::KeyRepeat(int keyCondition)
{
	// キーが押されたフレーム数
	static int elapseFrame = 0;

	// キーが押されているか
	static bool keyHeld = false;

	int keyRepeat = 0;

	// 簡易キーリピート
	if (keyCondition)
	{
		// 押された瞬間はキー情報を反映する
		if (!keyHeld) keyRepeat = keyCondition;
		// キーが押されている
		keyHeld = true;
		// リピート間隔を超えた
		if (++elapseFrame >= KEY_REPEAT_INTEVAL)
		{
			// キーが押されたフレーム数を初期化
			elapseFrame = 0;
			// キー情報を反映する
			keyRepeat = keyCondition;
		}
	}
	else
	{
		// キーが押されたフレーム数を初期化
		elapseFrame = 0;
		// キーが押されていない
		keyHeld = false;
	}

	return keyRepeat;
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

	// Aキーでカーソル位置にタイルを置く
	if (keyCondition & PAD_INPUT_4)
	{
		m_stage.SetTileType(m_cursorEdit.x, m_cursorEdit.y, m_selectTile);
	}
	// Wキーでカーソル位置のタイルを消す
	if (keyCondition & PAD_INPUT_8)
	{
		m_stage.SetTileType(m_cursorEdit.x, m_cursorEdit.y, Tile::TileType::None);
	}
	// SキーとDキーで選択タイルの変更
	if (keyRepeat & PAD_INPUT_5)
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
}

// セーブ
void StageEditScene::Save(int keyTrigger, int keyRepeat)
{
	// スペースキーでセーブ
	if (keyTrigger & PAD_INPUT_10)
	{
		m_stage.SaveLevel(m_level);
	}
	// 上キーまたはWキーでレベル加算
	if (keyRepeat & PAD_INPUT_UP || keyRepeat & PAD_INPUT_8)
	{
		if (m_level < STAGE_MAX) m_level++;
	}
	// 下キーまたはSキーでレベル減算
	if (keyRepeat & PAD_INPUT_DOWN || keyRepeat & PAD_INPUT_5)
	{
		if (m_level > 1) m_level--;
	}
	// レベルの設定
	m_levelNumber.SetNumber(m_level);
}

// ロード
void StageEditScene::Load(int keyTrigger, int keyRepeat)
{
	// スペースキーでロード
	if (keyTrigger & PAD_INPUT_10)
	{
		m_stage.LoadLevel(m_level);
	}
	// 上キーまたはWキーでレベル加算
	if (keyRepeat & PAD_INPUT_UP || keyRepeat & PAD_INPUT_8)
	{
		if (m_level < STAGE_MAX) m_level++;
	}
	// 下キーまたはSキーでレベル減算
	if (keyRepeat & PAD_INPUT_DOWN || keyRepeat & PAD_INPUT_5)
	{
		if (m_level > 1) m_level--;
	}
	// レベルの設定
	m_levelNumber.SetNumber(m_level);
}


