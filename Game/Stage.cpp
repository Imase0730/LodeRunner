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
	// �G�̃��[�h
	m_ghGame_bottom = LoadGraph(L"Resources/Textures/game_bottom.png");
	m_ghEdit_bottom = LoadGraph(L"Resources/Textures/edit_bottom.png");
}

Stage::~Stage()
{
	// �G�̃f�[�^������������폜
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
	// �X�e�[�W�̕`��
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			m_tileMap[i][j].Render(j * Tile::TILE_WIDTH, i * Tile::TILE_HEIGHT, ghTileset);
		}
	}

	// �����̕�����̕`��
	if (m_mode == Mode::GamePlay)
	{
		// ----- �Q�[���v���C ----- //
		DrawGraph(0, Tile::TILE_HEIGHT * STAGE_HEIGHT, m_ghGame_bottom, TRUE);

		// �X�R�A�̕\��
		m_scoreNumber.Render(ghTileset);

		// �c�@���̕\��
		m_menNumber.Render(ghTileset);

		// ���x���̕\��
		m_levelNumber.Render(ghTileset);
	}
	else
	{
		// ----- �X�e�[�W�G�f�B�b�g ----- //
		DrawGraph(0, Tile::TILE_HEIGHT * STAGE_HEIGHT, m_ghEdit_bottom, TRUE);
	}

}

// �w�背�x�����Z�[�u����֐�
bool Stage::SaveLevel(int level)
{
	char fileName[MAX_PATH];

	snprintf(fileName, MAX_PATH, "%sstage_%03d.data", STAGE_DATA_PATH, level);

	std::ofstream ofs(fileName);
	if (!ofs)
	{
		return false;
	}
	
	// �X�e�[�W�f�[�^���Z�[�u
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			Tile::TileType tileType = m_tileMap[i][j].GetTileType();
			ofs << static_cast<int>(tileType) << ",";
		}
		ofs << std::endl;
	}

	//�t�@�C�������
	ofs.close();

	return true;
}

// �w�背�x�������[�h����֐�
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

	//�t�@�C�������
	ifs.close();

	// ���x����ݒ�
	m_level = level;
	m_levelNumber.SetNumber(m_level);

	return false;
}

