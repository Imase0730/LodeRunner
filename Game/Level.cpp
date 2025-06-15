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
			m_page1[i][j] = Tile::Type::Empty;
			m_page2[i][j] = Tile::Type::Empty;
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
			Tile::Type type = m_loadData[i][j];

			// プレイヤーなら位置を取得
			if (type == Tile::Type::Player) m_playerPosition = POINT{ j, i };

			// ガードなら
			if ((type == Tile::Type::Guard) && (m_guardCount < GUARD_MAX - 1))
			{
				// ガードの位置を記憶
				m_guardPosition[m_guardCount] = POINT{ j, i };
				// ガードの数を加算
				m_guardCount++;
			}

			// 金塊なら金塊の数を加算
			if (type == Tile::Type::Gold) m_goldCount++;

			// 隠しハシゴなら
			if ((type == Tile::Type::InvisibleLadder) && (m_invisibleLadderCount < INVISIBLE_LADDER_MAX - 1))
			{
				// 隠しハシゴの位置を記憶する
				m_invisibleLadderPosition[m_invisibleLadderCount] = POINT{ j, i };
				// 隠しハシゴの数を加算
				m_invisibleLadderCount++;
			}

			// ステージデータを作成
			if ( (m_mode != Mode::GamePlay)
			  || ((type != Tile::Type::Player) && (type != Tile::Type::Guard) && (type != Tile::Type::InvisibleLadder))
			   )
			{
				m_page1[i][j] = m_page2[i][j] = type;
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
			int x = j * Tile::TILE_PIXEL_WIDTH;
			int y = i * Tile::TILE_PIXEL_HEIGHT;

			// ページ２を描画対象にする
			Tile::Type type = m_page2[i][j];

			// ゲームプレイなら
			if (m_mode == Mode::GamePlay)
			{
				// 罠はレンガで描画
				if (type == Tile::Type::Trap) type = Tile::Type::Blick;

				// Page2：レンガ　Page1：空白の場合、掘った穴なので空白を描画
				if ((m_page2[i][j] == Tile::Type::Blick) && (m_page1[i][j] == Tile::Type::Empty)) type = Tile::Type::Empty;
			}

			// タイルの絵の位置
			POINT pos = TILE_SPRITES[static_cast<int>(type)];
			// タイルを描画
			DrawRectGraph(x, y
				, Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		}
	}

	// ステージの下部の描画
	for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
	{
		DrawRectGraph(j * Tile::TILE_PIXEL_WIDTH, (MAX_GAME_ROW + 1) * Tile::TILE_PIXEL_HEIGHT
			, Tile::TILE_PIXEL_WIDTH * 2, Tile::TILE_PIXEL_HEIGHT * 4, Tile::TILE_PIXEL_WIDTH, 4, ghTileset, FALSE);
	}

	//// ステージの描画
	//for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	//{
	//	for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
	//	{
	//		// 描画位置
	//		int x = j * Tile::TILE_PIXEL_WIDTH;
	//		int y = i * Tile::TILE_PIXEL_HEIGHT;

	//		// ページ２のタイルを描画対象にする
	//		Tile::Type tile = m_page1[i][j];

	//		// タイルの絵の位置
	//		POINT pos = TILE_SPRITES[static_cast<int>(tile)];
	//		// タイルを描画
	//		DrawRectGraph(x, y
	//			, Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
	//			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
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
			m_loadData[i][j] = static_cast<Tile::Type>(std::stoi(item));
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
		m_page2[pos.y][pos.x] = Tile::Type::Ladder;
	}
}

// Page2の内容をPage1に指定位置のタイルをコピーする関数
void Level::CopyPage2toPage1(int x, int y)
{
	// 穴の上なのでコピーしない
	if (m_page2[y][x] == Tile::Type::Blick)
	{
		m_page1[y][x] = Tile::Type::Empty;
	}
	else
	{
		m_page1[y][x] = m_page2[y][x];
	}
}

