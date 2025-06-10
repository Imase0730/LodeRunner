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
	, m_levelNo{ 0 }
	, m_guardCount{ 0 }
	, m_goldCount{ 0 }
	, m_playerPosition{ 0, 0 }
	, m_guardPosition{}
	, m_digBrick{}
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
	if (levelNo != m_levelNo) LoadLevel(levelNo, mode);

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

	// ロードしたデータからステージデータを作成
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
		{
			Tile tile = m_loadData[i][j];

			// プレイヤーなら位置を取得
			if (tile == Tile::Player) m_playerPosition = POINT{ j, i };

			// ガードなら位置を取得
			if ((m_guardCount < GUARD_MAX - 1) && (tile == Tile::Guard))
			{
				m_guardPosition[m_guardCount] = POINT{ j, i };
				m_guardCount++;
			}

			// 金塊なら金塊の数を加算
			if (tile == Tile::Gold) m_goldCount++;

			// ステージデータを作成
			if ( (m_mode != Mode::GamePlay)
			  || ((tile != Tile::Player) && (tile != Tile::Guard) && (tile != Tile::InvisibleLadder))
			   )
			{
				m_page1[i][j] = m_page2[i][j] = tile;
			}
		}
	}

	// 掘ったレンガを情報の初期化
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		m_digBrick[i].position = POINT{0, 0};
		m_digBrick[i].timer = 0;
	}
}

// 更新処理
void Level::Update()
{
	//// 掘ったブロックを元に戻す
	//for (int i = 0; i < DIG_BRICK_MAX; i++)
	//{
	//	// 復元タイマーが０でない
	//	if (m_digBrick[i].timer != 0)
	//	{
	//		// 復元タイマーを減算する
	//		m_digBrick[i].timer--;

	//		// 復元するレンガの位置
	//		int x = m_digBrick[i].position.x;
	//		int y = m_digBrick[i].position.y;

	//		// レンガの復元アニメーション
	//		if (m_digBrick[i].timer == BRICK_ANIME_TIME_FILL01) m_stageData[y][x].SetDigAnimationState(Tile::DigAnimationState::Fill01);
	//		if (m_digBrick[i].timer == BRICK_ANIME_TIME_FILL02) m_stageData[y][x].SetDigAnimationState(Tile::DigAnimationState::Fill02);
	//		if (m_digBrick[i].timer == 0) m_stageData[y][x].SetDigAnimationState(Tile::DigAnimationState::NotDigging);
	//		if (m_digBrick[i].timer == 0) m_stageData[y][x].SetTileType(Tile::TileType::Blick);
	//	}
	//}

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

			Tile tile = m_page2[i][j];

			// ゲームプレイなら
			if (m_mode == Mode::GamePlay)
			{
				// 罠はレンガで表示する
				if (m_page2[i][j] == Tile::Trap) tile = Tile::Blick;

				// プレイヤーが穴を開けた場所
				if ((m_page2[i][j] == Tile::Blick) && (m_page1[i][j] == Tile::Empty))
				{
					tile = Tile::Empty;
				}
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
}

// 指定レベルをセーブする関数
bool Level::SaveLevel(int level)
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
	m_levelNo = level;

	return false;
}

// ハシゴを出現する関数
void Level::AppearLadder()
{
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
		{
			if (m_loadData[i][j] == Tile::InvisibleLadder)
			{
				m_page2[i][j] = Tile::Ladder;
			}
		}
	}
}

// 指定位置のレンガを復元する
void Level::SetFillBrick(int x, int y)
{
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		// 未使用のワークなら
		if (m_digBrick[i].timer == 0)
		{
			// レンガの復元情報を設定
			m_digBrick[i].position.x = x;
			m_digBrick[i].position.y = y;
			m_digBrick[i].timer = BRICK_FILL_FRAME;
			return;
		}
	}
}

