//--------------------------------------------------------------------------------------
// File: Level.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Level.h"
#include "Game/Game.h"
#include <fstream>
#include <sstream>

// コンストラクタ
Level::Level(Mode mode)
	: m_loadData{}
	, m_page1{}
	, m_page2{}
	, m_mode{ mode }
	, m_levelId{ 0 }
	, m_guardCount{ 0 }
	, m_goldCount{ 0 }
	, m_invisibleLadderCount{ 0 }
	, m_playerPosition{ 0, 0 }
	, m_guardPosition{}
	, m_invisibleLadderPosition{}
{
}

// デストラクタ
Level::~Level()
{
}

// 初期化処理
void Level::Initialize(int levelNo, Mode mode)
{
	// レベルが違っていればレベルをロードする
	if (levelNo != m_levelId) LoadLevel(levelNo, mode);

	// ステージデータの初期化
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN+ 1; j++)
		{
			m_page1[i][j] = Tile::Empty;
			m_page2[i][j] = Tile::Empty;
		}
	}

	// モードを設定
	m_mode = mode;

	// ガードの数を初期化
	m_guardCount = 0;

	// 金塊の数を初期化
	m_goldCount = 0;

	// 隠しハシゴの数を初期化
	m_invisibleLadderCount = 0;

	// ロードしたデータからステージデータを作成
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
		{
			// タイルの種類
			Tile tile = m_loadData[i][j];

			// プレイヤーなら位置を取得
			if (tile == Tile::Player) m_playerPosition = POINT{ j, i };

			// ガードなら
			if ((tile == Tile::Guard) && (m_guardCount < GUARD_MAX - 1))
			{
				// ガードの位置を記憶
				m_guardPosition[m_guardCount] = POINT{ j, i };
				// ガードの数を加算
				m_guardCount++;
			}

			// 金塊なら金塊の数を加算
			if (tile == Tile::Gold) m_goldCount++;

			// 隠しハシゴなら
			if ((tile == Tile::InvisibleLadder) && (m_invisibleLadderCount < INVISIBLE_LADDER_MAX - 1))
			{
				// 隠しハシゴの位置を記憶する
				m_invisibleLadderPosition[m_invisibleLadderCount] = POINT{ j, i };
				// 隠しハシゴの数を加算
				m_invisibleLadderCount++;
			}

			// ステージデータを作成
			if ( (m_mode != Mode::GamePlay)
			  || ((tile != Tile::Player) && (tile != Tile::Guard) && (tile != Tile::InvisibleLadder))
			   )
			{
				m_page1[i][j] = m_page2[i][j] = tile;
			}
		}
	}
}

// 更新処理
void Level::Update()
{
	// 金塊が全てなくなったら隠れハシゴを出現させる
	if (m_goldCount == 0)
	{
		// 何度も出現処理をしないように、金塊の数を引いておく
		m_goldCount--;
		AppearLadder();
	}
}

// 描画処理
void Level::Render(int ghTileset) const
{
	// ステージの描画
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
		{
			// 描画位置
			int x = j * TILE_PIXEL_WIDTH;
			int y = i * TILE_PIXEL_HEIGHT;

			// ページ２を描画対象にする
			Tile tile = m_page2[i][j];

			// ゲームプレイなら
			if (m_mode == Mode::GamePlay)
			{
				// 罠はレンガで描画
				if (tile == Tile::Trap) tile = Tile::Blick;

				// Page2：レンガ　Page1：空白の場合、掘った穴なので空白を描画
				if ((m_page2[i][j] == Tile::Blick) && (m_page1[i][j] == Tile::Empty)) tile = Tile::Empty;
			}

			// タイルの絵の位置
			POINT pos = TILE_SPRITES[static_cast<int>(tile)];
			// タイルを描画
			DrawRectGraph(x, y
				, TILE_PIXEL_WIDTH * pos.x, TILE_PIXEL_HEIGHT * pos.y
				, TILE_PIXEL_WIDTH, TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		}
	}

	// ステージの下部の描画
	for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
	{
		DrawRectGraph(j * TILE_PIXEL_WIDTH, (MAX_GAME_ROW + 1) * TILE_PIXEL_HEIGHT
			, TILE_PIXEL_WIDTH * 2, TILE_PIXEL_HEIGHT * 4, TILE_PIXEL_WIDTH, 4, ghTileset, FALSE);
	}

	//// ステージの描画
	//for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	//{
	//	for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
	//	{
	//		// 描画位置
	//		int x = j * TILE_PIXEL_WIDTH;
	//		int y = i * TILE_PIXEL_HEIGHT;

	//		// ページ２のタイルを描画対象にする
	//		Tile tile = m_page1[i][j];

	//		// タイルの絵の位置
	//		POINT pos = TILE_SPRITES[static_cast<int>(tile)];
	//		// タイルを描画
	//		DrawRectGraph(x, y
	//			, TILE_PIXEL_WIDTH * pos.x, TILE_PIXEL_HEIGHT * pos.y
	//			, TILE_PIXEL_WIDTH, TILE_PIXEL_HEIGHT, ghTileset, TRUE);
	//	}
	//}

}

// 指定レベルをセーブする関数
bool Level::SaveLevel(int level) const
{
	char fileName[MAX_PATH];

	snprintf(fileName, MAX_PATH, "%slevel_%03d.csv", LEVEL_DATA_PATH, level);

	std::ofstream ofs(fileName);
	if (!ofs)
	{
		return false;
	}
	
	// ステージデータをセーブ
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
		{
			ofs << static_cast<int>(m_page2[i][j]) << ",";
		}
		ofs << std::endl;
	}

	//ファイルを閉じる
	ofs.close();

	return true;
}

// 指定レベルをロードする関数
bool Level::LoadLevel(int level, Mode mode)
{
	char fileName[MAX_PATH];

	snprintf(fileName, MAX_PATH, "%slevel_%03d.csv", LEVEL_DATA_PATH, level);

	std::ifstream ifs(fileName);
	if (!ifs)
	{
		return false;
	}

	std::string line;

	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		std::getline(ifs, line);
		std::stringstream ss(line);
		std::string item;

		for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
		{
			std::getline(ss, item, ',');
			m_loadData[i][j] = static_cast<Tile>(std::stoi(item));
		}
	}

	//ファイルを閉じる
	ifs.close();

	// レベルを設定
	m_levelId = level;

	return false;
}

// 隠しハシゴを出現する関数
void Level::AppearLadder()
{
	for (int i = 0; i < m_invisibleLadderCount; i++)
	{
		POINT pos = m_invisibleLadderPosition[i];
		m_page2[pos.y][pos.x] = Tile::Ladder;
	}
}

// 移動可能なタイルか調べる関数（上左右）
bool Level::IsMovableTileULR(Level::Tile tile)
{
	// ブロック、石、罠なら移動不可
	if ((tile == Level::Tile::Blick)
		|| (tile == Level::Tile::Stone)
		|| (tile == Level::Tile::Trap)
		)
	{
		return false;
	}
	return true;
}

// 移動可能なタイルか調べる関数（下）
bool Level::IsMovableTileDown(Level::Tile tile)
{
	// ブロック、石なら移動不可
	if ((tile == Level::Tile::Blick)
		|| (tile == Level::Tile::Stone)
		)
	{
		return false;
	}
	return true;
}

// Page2の内容をPage1に指定位置のタイルをコピーする関数
void Level::CopyPage2toPage1(int x, int y)
{
	// 穴の上なのでコピーしない
	if (m_page2[y][x] == Level::Tile::Blick)
	{
		m_page1[y][x] = Tile::Empty;
	}
	else
	{
		m_page1[y][x] = m_page2[y][x];
	}
}

