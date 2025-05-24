//--------------------------------------------------------------------------------------
// File: Stage.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Stage.h"
#include "Game/Game.h"
#include <fstream>
#include <sstream>

Stage::Stage(Mode mode)
	: m_mode{ mode }
	, m_level{ 0 }
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
}

void Stage::Update()
{
}

void Stage::Render(int ghTileset) const
{
	// ステージの描画
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			m_tileMap[i][j].Render(j * Tile::TILE_WIDTH, i * Tile::TILE_HEIGHT, ghTileset);
		}
	}
	// ステージの下部の描画
	for (int j = 0; j < STAGE_WIDTH; j++)
	{
		DrawRectGraph(j * Tile::TILE_WIDTH, STAGE_HEIGHT * Tile::TILE_HEIGHT
			, Tile::TILE_WIDTH * 2, Tile::TILE_HEIGHT * 4, Tile::TILE_WIDTH, 4, ghTileset, FALSE);
	}
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
			Tile::TileType tileType = m_tileMap[i][j].GetTileType();
			ofs << static_cast<int>(tileType) << ",";
		}
		ofs << std::endl;
	}

	//ファイルを閉じる
	ofs.close();

	return true;
}

// 指定レベルをロードする関数
bool Stage::LoadLevel(int level)
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
			m_tileMap[i][j].SetTileType(static_cast<Tile::TileType>(std::stoi(item)));
		}
	}

	//ファイルを閉じる
	ifs.close();

	// レベルを設定
	m_level = level;

	return false;
}

