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
	, m_cursorEdit{ 0, 0 }
	, m_selectTile{ Tile::TileType::Blick }
	, m_blink{ BLINK_INTEVAL }
	, m_level{ 1 }
	, m_mode{ Mode::SelectTile }
	, m_saveString{ POINT{ Tile::TILE_PIXEL_WIDTH * 10 , Game::INFOMATION_Y }, "SAVE" }
	, m_loadString{ POINT{ Tile::TILE_PIXEL_WIDTH * 15 , Game::INFOMATION_Y }, "LOAD" }
	, m_levelString{ POINT{ Tile::TILE_PIXEL_WIDTH * 20 , Game::INFOMATION_Y }, "LEVEL" }
	, m_levelNumber{ POINT{ Tile::TILE_PIXEL_WIDTH * 25, Game::INFOMATION_Y }, 3 }
{

}

// デストラクタ
StageEditScene::~StageEditScene()
{
}

// 初期化処理
void StageEditScene::Initialize()
{
	// 各変数の初期化
	m_cursorEdit = POINT{ 0, 0 };
	m_selectTile = Tile::TileType::Blick;
	m_level = 1;
	m_mode = Mode::SelectTile;

	// レベルの設定
	m_levelNumber.SetNumber(m_level);

	// ステージのロード
	m_stage.LoadLevel(m_level, Stage::Mode::StageEdit);
}

// 更新処理
void StageEditScene::Update(int keyCondition, int keyTrigger)
{
	// Qキーでタイトルへ
	if (keyTrigger & PAD_INPUT_7)
	{
		m_pGame->RequestSceneChange(Game::SceneID::Title);
		return;
	}

	// 簡易キーリピート
	int keyRepeat = KeyRepeat(keyCondition);

	// 点滅の更新
	m_blink.Update();

	// スペースキーでモード変更
	if (keyTrigger & PAD_INPUT_10)
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
void StageEditScene::Render(int ghTileset)
{
	// ステージの描画
	m_stage.Render(ghTileset);

	// 選択用タイルの描画
	DrawRectGraph(0, Game::INFOMATION_Y
		, Tile::TILE_PIXEL_WIDTH * 1, Tile::TILE_PIXEL_HEIGHT * 4
		, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);

	// 各文字列の描画
	if (m_mode != Mode::Save) m_saveString.Render(ghTileset);			// SAVE
	if (m_mode != Mode::Load) m_loadString.Render(ghTileset);			// LOAD
	m_levelString.Render(ghTileset);									// LEVEL
	if (m_mode == Mode::SelectTile) m_levelNumber.Render(ghTileset);	// 000

	// ----- ここから点滅 ----- //
	int col = static_cast<int>(100.0f + 155.0f * m_blink.GetBlinkRate());
	SetDrawBright(col, col, col);

	switch (m_mode)
	{
	case Mode::SelectTile:
		// カーソルの描画（上部）
		DrawRectGraph(m_cursorEdit.x * Tile::TILE_PIXEL_WIDTH, m_cursorEdit.y * Tile::TILE_PIXEL_HEIGHT
			, Tile::TILE_PIXEL_WIDTH * 6, Tile::TILE_PIXEL_HEIGHT * 3
			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);

		// カーソルの描画（下部）
		DrawRectGraph(Tile::TILE_PIXEL_WIDTH * (static_cast<int>(m_selectTile) - 1), Game::INFOMATION_Y
			, Tile::TILE_PIXEL_WIDTH * 6, Tile::TILE_PIXEL_HEIGHT * 3
			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		break;
	case Mode::Save:
		m_saveString.Render(ghTileset);	// SAVE
		m_levelNumber.Render(ghTileset);	// 000
		break;
	case Mode::Load:
		m_loadString.Render(ghTileset);	// LOAD
		m_levelNumber.Render(ghTileset);	// 000
		break;
	default:
		break;
	}

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
	// １フレーム前のキー情報
	static int oldKey = 0;

	int keyRepeat = 0;

	// キーが押されている
	if (keyCondition)
	{
		// キーが押された瞬間はキー情報を反映する
		if (~oldKey & keyCondition)
		{
			keyRepeat = keyCondition;
			elapseFrame = 0;
		}
		// リピート間隔を超えた
		if (++elapseFrame >= KEY_REPEAT_INTEVAL)
		{
			elapseFrame = 0;
			keyRepeat = keyCondition;
		}
	}

	oldKey = keyCondition;

	return keyRepeat;
}

// タイル選択
void StageEditScene::SelectTile(int keyCondition, int keyRepeat)
{
	// Cキーと左右キーで選択タイルの変更
	if (keyCondition & PAD_INPUT_3)
	{
		if (keyRepeat & PAD_INPUT_LEFT)
		{
			if (m_selectTile != Tile::TileType::Blick)
			{
				m_selectTile = static_cast<Tile::TileType>((static_cast<int>(m_selectTile) - 1));
			}
		}
		if (keyRepeat & PAD_INPUT_RIGHT)
		{
			if (m_selectTile != Tile::TileType::Player)
			{
				m_selectTile = static_cast<Tile::TileType>((static_cast<int>(m_selectTile) + 1));
			}
		}
		return;
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

	// Aキーでカーソル位置にタイルを置く
	if (keyCondition & PAD_INPUT_1)
	{
		m_stage.SetTileType(m_cursorEdit.x, m_cursorEdit.y, m_selectTile);
	}
	// Sキーでカーソル位置のタイルを消す
	if (keyCondition & PAD_INPUT_2)
	{
		m_stage.SetTileType(m_cursorEdit.x, m_cursorEdit.y, Tile::TileType::Empty);
	}
}

// セーブ
void StageEditScene::Save(int keyTrigger, int keyRepeat)
{
	// Zキーでセーブ
	if (keyTrigger & PAD_INPUT_1)
	{
		m_stage.SaveLevel(m_level);
	}
	// 上キーまたはWキーでレベル加算
	if ((keyRepeat & PAD_INPUT_UP) || (keyRepeat & PAD_INPUT_8))
	{
		if (m_level < STAGE_MAX) m_level++;
	}
	// 下キーまたはSキーでレベル減算
	if ((keyRepeat & PAD_INPUT_DOWN) || (keyRepeat & PAD_INPUT_5))
	{
		if (m_level > 1) m_level--;
	}
	// レベルの設定
	m_levelNumber.SetNumber(m_level);
}

// ロード
void StageEditScene::Load(int keyTrigger, int keyRepeat)
{
	// Zキーでロード
	if (keyTrigger & PAD_INPUT_1)
	{
		if (m_level != m_stage.GetLevel())
		{
			m_stage.LoadLevel(m_level, Stage::Mode::StageEdit);
		}
	}
	// 上キーまたはWキーでレベル加算
	if ((keyRepeat & PAD_INPUT_UP) || (keyRepeat & PAD_INPUT_8))
	{
		if (m_level < STAGE_MAX) m_level++;
	}
	// 下キーまたはSキーでレベル減算
	if ((keyRepeat & PAD_INPUT_DOWN) || (keyRepeat & PAD_INPUT_5))
	{
		if (m_level > 1) m_level--;
	}
	// レベルの設定
	m_levelNumber.SetNumber(m_level);
}


