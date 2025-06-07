//--------------------------------------------------------------------------------------
// File: Stage.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Stage.h"
#include "Game/Game.h"
#include <fstream>
#include <sstream>

// コンストラクタ
Stage::Stage(Mode mode)
	: m_loadData{}
	, m_stageData{}
	, m_mode{ mode }
	, m_level{ 0 }
	, m_guardCount{ 0 }
	, m_goldCount{ 0 }
	, m_playerPosition{ 0, 0 }
	, m_guardPosition{}
	, m_digBrick{}
{
}

// デストラクタ
Stage::~Stage()
{
}

// 初期化処理
void Stage::Initialize(int level, Mode mode)
{
	// レベルが違っていればレベルをロードする
	if (level != m_level) LoadLevel(level, mode);

	// ステージデータの初期化
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			m_stageData[i][j].SetTileType(Tile::TileType::Empty);
		}
	}

	// モードを設定
	m_mode = mode;

	// ガードの数を初期化
	m_guardCount = 0;

	// 金塊の数を初期化
	m_goldCount = 0;

	// ロードしたデータからステージデータを作成
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			Tile::TileType type = m_loadData[i][j];

			// プレイヤーなら位置を取得
			if (type == Tile::TileType::Player) m_playerPosition = POINT{ j, i };

			// ガードなら位置を取得
			if ((m_guardCount < GUARD_MAX - 1) && (type == Tile::TileType::Guard))
			{
				m_guardPosition[m_guardCount] = POINT{ j, i };
				m_guardCount++;
			}

			// 金塊なら金塊の数を加算
			if (type == Tile::TileType::Gold) m_goldCount++;

			// ステージデータを作成
			if ( (m_mode != Mode::GamePlay)
			  || ((type != Tile::TileType::Player) && (type != Tile::TileType::Guard))
			   )
			{
				m_stageData[i][j].SetTileType(type);
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
void Stage::Update()
{
	// 掘ったブロックを元に戻す
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		// 復元タイマーが０でない
		if (m_digBrick[i].timer != 0)
		{
			// 復元タイマーを減算する
			m_digBrick[i].timer--;

			// 復元するレンガの位置
			int x = m_digBrick[i].position.x;
			int y = m_digBrick[i].position.y;

			// レンガの復元アニメーション
			if (m_digBrick[i].timer == BRICK_ANIME_TIME_FILL01) m_stageData[y][x].SetDigAnimationState(Tile::DigAnimationState::Fill01);
			if (m_digBrick[i].timer == BRICK_ANIME_TIME_FILL02) m_stageData[y][x].SetDigAnimationState(Tile::DigAnimationState::Fill02);
			if (m_digBrick[i].timer == 0) m_stageData[y][x].SetDigAnimationState(Tile::DigAnimationState::NotDigging);
			if (m_digBrick[i].timer == 0) m_stageData[y][x].SetTileType(Tile::TileType::Blick);
		}
	}

	// 金塊が全てなくなったら隠れハシゴを出現させる
	if (m_goldCount == 0)
	{
		// 何度も出現処理をしないように、金塊の数を引いておく
		m_goldCount--;
		AppearLadder();
	}

}

// 描画処理
void Stage::Render(int ghTileset) const
{
	// 罠をレンガに変更するためのタイル
	Tile trapTile;
	trapTile.SetTileType(Tile::TileType::Blick);

	// ステージの描画
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			// 描画位置
			int x = j * Tile::TILE_PIXEL_WIDTH;
			int y = i * Tile::TILE_PIXEL_HEIGHT;

			// ゲームプレイなら
			if (m_mode == Mode::GamePlay)
			{
				// 隠れハシゴは表示しない
				if (m_stageData[i][j].GetTileType() == Tile::TileType::InvisibleLadder) continue;

				// 罠はレンガで表示する
				if (m_stageData[i][j].GetTileType() == Tile::TileType::Trap)
				{
					trapTile.Render(x, y, ghTileset);
					continue;
				}
			}
			// タイルを描画
			m_stageData[i][j].Render(x, y, ghTileset);
		}
	}

	// ステージの下部の描画
	for (int j = 0; j < STAGE_WIDTH; j++)
	{
		DrawRectGraph(j * Tile::TILE_PIXEL_WIDTH, STAGE_HEIGHT * Tile::TILE_PIXEL_HEIGHT
			, Tile::TILE_PIXEL_WIDTH * 2, Tile::TILE_PIXEL_HEIGHT * 4, Tile::TILE_PIXEL_WIDTH, 4, ghTileset, FALSE);
	}
}

// 指定場所のタイルを設定する関数
void Stage::SetTileType(int x, int y, Tile::TileType type)
{
	// 金塊がある場合は金塊の数を減らす
	if (m_stageData[y][x].GetTileType() == Tile::TileType::Gold) m_goldCount--;

	// タイルを設定する
	m_stageData[y][x].SetTileType(type);
}

// 指定場所のタイルを取得する関数
Tile::TileType Stage::GetTileType(int x, int y)
{
	if ( (x < 0) || (x >= Stage::STAGE_WIDTH)
	  || (y < 0) || (y >= Stage::STAGE_HEIGHT)
	   )
	{
		// ステージ外の場合、石を返す
		return Tile::TileType::Stone;
	}
	return m_stageData[y][x].GetTileType();
}

// 指定レベルをセーブする関数
bool Stage::SaveLevel(int level)
{
	char fileName[MAX_PATH];

	snprintf(fileName, MAX_PATH, "%sstage_%03d.csv", STAGE_DATA_PATH, level);

	std::ofstream ofs(fileName);
	if (!ofs)
	{
		return false;
	}
	
	// ステージデータをセーブ
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			Tile::TileType tileType = m_stageData[i][j].GetTileType();
			ofs << static_cast<int>(tileType) << ",";
		}
		ofs << std::endl;
	}

	//ファイルを閉じる
	ofs.close();

	return true;
}

// 指定レベルをロードする関数
bool Stage::LoadLevel(int level, Mode mode)
{
	char fileName[MAX_PATH];

	snprintf(fileName, MAX_PATH, "%sstage_%03d.csv", STAGE_DATA_PATH, level);

	std::ifstream ifs(fileName);
	if (!ifs)
	{
		return false;
	}

	std::string line;

	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		std::getline(ifs, line);
		std::stringstream ss(line);
		std::string item;

		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			std::getline(ss, item, ',');
			m_loadData[i][j] = static_cast<Tile::TileType>(std::stoi(item));
		}
	}

	//ファイルを閉じる
	ifs.close();

	// レベルを設定
	m_level = level;

	return false;
}

// ハシゴを出現する関数
void Stage::AppearLadder()
{
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (m_stageData[i][j].GetTileType() == Tile::TileType::InvisibleLadder)
			{
				m_stageData[i][j].SetTileType(Tile::TileType::Ladder);
			}
		}
	}
}

// 指定位置のレンガを復元する
void Stage::SetFillBrick(int x, int y)
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

