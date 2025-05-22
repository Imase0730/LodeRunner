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
	, m_ghGame_bottom{ -1 }
	, m_ghEdit_bottom{ -1 }
	, m_scoreNumber{ POINT{ 5 * Tile::TILE_WIDTH, Screen::GAME_HEIGHT - Tile::TILE_HEIGHT }, 7 }
	, m_menNumber{ POINT{ 16 * Tile::TILE_WIDTH, Screen::GAME_HEIGHT - Tile::TILE_HEIGHT }, 3 }
	, m_levelNumber{ POINT{ 25 * Tile::TILE_WIDTH, Screen::GAME_HEIGHT - Tile::TILE_HEIGHT }, 3 }
	, m_level{ 0 }
{
	// 絵のロード
	m_ghGame_bottom = LoadGraph(L"Resources/Textures/game_bottom.png");
	m_ghEdit_bottom = LoadGraph(L"Resources/Textures/edit_bottom.png");
}

Stage::~Stage()
{
	// 絵のデータをメモリから削除
	DeleteGraph(m_ghGame_bottom);
	DeleteGraph(m_ghEdit_bottom);
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

	// 下部の文字列の描画
	if (m_mode == Mode::GamePlay)
	{
		// ----- ゲームプレイ ----- //
		DrawGraph(0, Tile::TILE_HEIGHT * STAGE_HEIGHT, m_ghGame_bottom, TRUE);

		// スコアの表示
		m_scoreNumber.Render(ghTileset);

		// 残機数の表示
		m_menNumber.Render(ghTileset);

		// レベルの表示
		m_levelNumber.Render(ghTileset);
	}
	else
	{
		// ----- ステージエディット ----- //
		DrawGraph(0, Tile::TILE_HEIGHT * STAGE_HEIGHT, m_ghEdit_bottom, TRUE);
	}

}

// 指定レベルをセーブする関数
bool Stage::SaveLevel(int level)
{
	char fileName[MAX_PATH];

	snprintf(fileName, MAX_PATH, "%sstage_%03d.data", STAGE_DATA_PATH, level);

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

	snprintf(fileName, MAX_PATH, "%sstage_%03d.data", STAGE_DATA_PATH, level);

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
	m_levelNumber.SetNumber(m_level);

	return false;
}

