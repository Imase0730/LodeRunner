//--------------------------------------------------------------------------------------
// File: LevelEditScene.cpp
//
//--------------------------------------------------------------------------------------
#include "LevelEditScene.h"
#include "Game/Game.h"

// コンストラクタ
LevelEditScene::LevelEditScene(Game* pGame)
	: m_pGame{ pGame }
	, m_level{ Level::Mode::LevelEdit }
	, m_cursorEdit{ 0, 0 }
	, m_selectTile{ Level::Tile::Blick }
	, m_blink{ BLINK_INTEVAL }
	, m_levelNo{ 0 }
	, m_mode{ Mode::SelectTile }
	, m_saveString{ POINT{ Level::TILE_PIXEL_WIDTH * 10 , Game::INFOMATION_Y }, "SAVE" }
	, m_loadString{ POINT{ Level::TILE_PIXEL_WIDTH * 15 , Game::INFOMATION_Y }, "LOAD" }
	, m_levelString{ POINT{ Level::TILE_PIXEL_WIDTH * 20 , Game::INFOMATION_Y }, "LEVEL" }
	, m_levelNumber{ POINT{ Level::TILE_PIXEL_WIDTH * 25, Game::INFOMATION_Y }, 3 }
{

}

// デストラクタ
LevelEditScene::~LevelEditScene()
{
}

// 初期化処理
void LevelEditScene::Initialize()
{
	// 各変数の初期化
	m_cursorEdit = POINT{ 0, 0 };
	m_selectTile = Level::Tile::Blick;
	m_levelNo = 1;
	m_mode = Mode::SelectTile;

	// レベルの数字の表示
	m_levelNumber.SetNumber(m_levelNo);

	// レベルの初期化
	m_level.Initialize(m_levelNo, Level::Mode::LevelEdit);
}

// 更新処理
void LevelEditScene::Update(int keyCondition, int keyTrigger)
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
void LevelEditScene::Render(int ghTileset)
{
	// ステージの描画
	m_level.Render(ghTileset);

	// 選択用タイルの描画
	DrawRectGraph(0, Game::INFOMATION_Y
		, Level::TILE_PIXEL_WIDTH * 1, Level::TILE_PIXEL_HEIGHT * 4
		, Level::TILE_PIXEL_WIDTH * 9, Level::TILE_PIXEL_HEIGHT, ghTileset, TRUE);

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
		DrawRectGraph(m_cursorEdit.x * Level::TILE_PIXEL_WIDTH, m_cursorEdit.y * Level::TILE_PIXEL_HEIGHT
			, Level::TILE_PIXEL_WIDTH * 6, Level::TILE_PIXEL_HEIGHT * 3
			, Level::TILE_PIXEL_WIDTH, Level::TILE_PIXEL_HEIGHT, ghTileset, TRUE);

		// カーソルの描画（下部）
		DrawRectGraph(Level::TILE_PIXEL_WIDTH * (static_cast<int>(m_selectTile) - 1), Game::INFOMATION_Y
			, Level::TILE_PIXEL_WIDTH * 6, Level::TILE_PIXEL_HEIGHT * 3
			, Level::TILE_PIXEL_WIDTH, Level::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
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
void LevelEditScene::Finalize()
{
}

// 簡易キーリピート
int LevelEditScene::KeyRepeat(int keyCondition)
{
	// キーが押されたフレーム数
	static int elapseFrame = 0;
	// １フレーム前のキー情報
	static int oldKey = 0;

	int keyTrigger = ~oldKey & keyCondition;
	int keyRepeat = 0;

	// キーが押されている
	if (keyCondition)
	{
		// キーが押された瞬間はキー情報を反映する
		if (keyTrigger)
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
void LevelEditScene::SelectTile(int keyCondition, int keyRepeat)
{
	// Dキーと左右キーで選択タイルの変更
	if (keyCondition & PAD_INPUT_6)
	{
		if (keyRepeat & PAD_INPUT_LEFT)
		{
			if (m_selectTile != Level::Tile::Blick)
			{
				m_selectTile = static_cast<Level::Tile>(static_cast<int>(m_selectTile) - 1);
			}
		}
		if (keyRepeat & PAD_INPUT_RIGHT)
		{
			if (m_selectTile != Level::Tile::Player)
			{
				m_selectTile = static_cast<Level::Tile>(static_cast<int>(m_selectTile) + 1);
			}
		}
		return;
	}

	// Wキーで選択タイルの変更（ループ）
	if (keyRepeat & PAD_INPUT_8)
	{
		m_selectTile = static_cast<Level::Tile>((static_cast<int>(m_selectTile) + 1) % SELECT_TILE_MAX);
		if (m_selectTile == Level::Tile::Empty) m_selectTile = Level::Tile::Blick;
	}

	// 上部カーソルの移動
	if (keyRepeat & PAD_INPUT_UP)    m_cursorEdit.y--;
	if (keyRepeat & PAD_INPUT_DOWN)  m_cursorEdit.y++;
	if (keyRepeat & PAD_INPUT_LEFT)  m_cursorEdit.x--;
	if (keyRepeat & PAD_INPUT_RIGHT) m_cursorEdit.x++;

	// 上部カーソルの画面外チェック
	if (m_cursorEdit.x < 0) m_cursorEdit.x = 0;
	if (m_cursorEdit.x > Level::MAX_GAME_COLMUN) m_cursorEdit.x = Level::MAX_GAME_COLMUN;
	if (m_cursorEdit.y < 0) m_cursorEdit.y = 0;
	if (m_cursorEdit.y > Level::MAX_GAME_ROW) m_cursorEdit.y = Level::MAX_GAME_ROW;

	// Aキーでカーソル位置にタイルを置く
	if (keyCondition & PAD_INPUT_4)
	{
		m_level.SetTilePage2(m_cursorEdit.x, m_cursorEdit.y, m_selectTile);
	}
	// Sキーでカーソル位置のタイルを消す
	if (keyCondition & PAD_INPUT_5)
	{
		m_level.SetTilePage2(m_cursorEdit.x, m_cursorEdit.y, Level::Tile::Empty);
	}
}

// セーブ
void LevelEditScene::Save(int keyTrigger, int keyRepeat)
{
	// Zキーでセーブ
	if (keyTrigger & PAD_INPUT_1)
	{
		m_level.SaveLevel(m_levelNo);
	}
	// 上キーまたはWキーでレベル加算
	if ((keyRepeat & PAD_INPUT_UP) || (keyRepeat & PAD_INPUT_8))
	{
		if (m_levelNo < LEVEL_MAX) m_levelNo++;
	}
	// 下キーまたはSキーでレベル減算
	if ((keyRepeat & PAD_INPUT_DOWN) || (keyRepeat & PAD_INPUT_5))
	{
		if (m_levelNo > 1) m_levelNo--;
	}
	// レベルの設定
	m_levelNumber.SetNumber(m_levelNo);
}

// ロード
void LevelEditScene::Load(int keyTrigger, int keyRepeat)
{
	// Zキーでロード
	if (keyTrigger & PAD_INPUT_1)
	{
		if (m_levelNo != m_level.GetLevel())
		{
			m_level.Initialize(m_levelNo, Level::Mode::LevelEdit);
		}
	}
	// 上キーまたはWキーでレベル加算
	if ((keyRepeat & PAD_INPUT_UP) || (keyRepeat & PAD_INPUT_8))
	{
		if (m_levelNo < LEVEL_MAX) m_levelNo++;
	}
	// 下キーまたはSキーでレベル減算
	if ((keyRepeat & PAD_INPUT_DOWN) || (keyRepeat & PAD_INPUT_5))
	{
		if (m_levelNo > 1) m_levelNo--;
	}
	// レベルの設定
	m_levelNumber.SetNumber(m_levelNo);
}


