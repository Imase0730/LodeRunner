//--------------------------------------------------------------------------------------
// File: Level.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Level.h"
#include "Game/Game.h"
#include <fstream>
#include <sstream>

// �R���X�g���N�^
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

// �f�X�g���N�^
Level::~Level()
{
}

// ����������
void Level::Initialize(int levelNo, Mode mode)
{
	// ���x��������Ă���΃��x�������[�h����
	if (levelNo != m_levelNo) LoadLevel(levelNo, mode);

	// �X�e�[�W�f�[�^�̏�����
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN+ 1; j++)
		{
			m_page1[i][j] = Tile::Empty;
			m_page2[i][j] = Tile::Empty;
		}
	}

	// ���[�h��ݒ�
	m_mode = mode;

	// �K�[�h�̐���������
	m_guardCount = 0;

	// ����̐���������
	m_goldCount = 0;

	// ���[�h�����f�[�^����X�e�[�W�f�[�^���쐬
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
		{
			Tile tile = m_loadData[i][j];

			// �v���C���[�Ȃ�ʒu���擾
			if (tile == Tile::Player) m_playerPosition = POINT{ j, i };

			// �K�[�h�Ȃ�ʒu���擾
			if ((m_guardCount < GUARD_MAX - 1) && (tile == Tile::Guard))
			{
				m_guardPosition[m_guardCount] = POINT{ j, i };
				m_guardCount++;
			}

			// ����Ȃ����̐������Z
			if (tile == Tile::Gold) m_goldCount++;

			// �X�e�[�W�f�[�^���쐬
			if ( (m_mode != Mode::GamePlay)
			  || ((tile != Tile::Player) && (tile != Tile::Guard) && (tile != Tile::InvisibleLadder))
			   )
			{
				m_page1[i][j] = m_page2[i][j] = tile;
			}
		}
	}

	// �@���������K�����̏�����
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		m_digBrick[i].position = POINT{0, 0};
		m_digBrick[i].timer = 0;
	}
}

// �X�V����
void Level::Update()
{
	//// �@�����u���b�N�����ɖ߂�
	//for (int i = 0; i < DIG_BRICK_MAX; i++)
	//{
	//	// �����^�C�}�[���O�łȂ�
	//	if (m_digBrick[i].timer != 0)
	//	{
	//		// �����^�C�}�[�����Z����
	//		m_digBrick[i].timer--;

	//		// �������郌���K�̈ʒu
	//		int x = m_digBrick[i].position.x;
	//		int y = m_digBrick[i].position.y;

	//		// �����K�̕����A�j���[�V����
	//		if (m_digBrick[i].timer == BRICK_ANIME_TIME_FILL01) m_stageData[y][x].SetDigAnimationState(Tile::DigAnimationState::Fill01);
	//		if (m_digBrick[i].timer == BRICK_ANIME_TIME_FILL02) m_stageData[y][x].SetDigAnimationState(Tile::DigAnimationState::Fill02);
	//		if (m_digBrick[i].timer == 0) m_stageData[y][x].SetDigAnimationState(Tile::DigAnimationState::NotDigging);
	//		if (m_digBrick[i].timer == 0) m_stageData[y][x].SetTileType(Tile::TileType::Blick);
	//	}
	//}

	// ���򂪑S�ĂȂ��Ȃ�����B��n�V�S���o��������
	if (m_goldCount == 0)
	{
		// ���x���o�����������Ȃ��悤�ɁA����̐��������Ă���
		m_goldCount--;
		AppearLadder();
	}
}

// �`�揈��
void Level::Render(int ghTileset) const
{
	// �X�e�[�W�̕`��
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
		{
			// �`��ʒu
			int x = j * TILE_PIXEL_WIDTH;
			int y = i * TILE_PIXEL_HEIGHT;

			Tile tile = m_page2[i][j];

			// �Q�[���v���C�Ȃ�
			if (m_mode == Mode::GamePlay)
			{
				// 㩂̓����K�ŕ\������
				if (m_page2[i][j] == Tile::Trap) tile = Tile::Blick;

				// �v���C���[�������J�����ꏊ
				if ((m_page2[i][j] == Tile::Blick) && (m_page1[i][j] == Tile::Empty))
				{
					tile = Tile::Empty;
				}
			}
			// �^�C���̊G�̈ʒu
			POINT pos = TILE_SPRITES[static_cast<int>(tile)];
			// �^�C����`��
			DrawRectGraph(x, y
				, TILE_PIXEL_WIDTH * pos.x, TILE_PIXEL_HEIGHT * pos.y
				, TILE_PIXEL_WIDTH, TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		}
	}

	// �X�e�[�W�̉����̕`��
	for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
	{
		DrawRectGraph(j * TILE_PIXEL_WIDTH, (MAX_GAME_ROW + 1) * TILE_PIXEL_HEIGHT
			, TILE_PIXEL_WIDTH * 2, TILE_PIXEL_HEIGHT * 4, TILE_PIXEL_WIDTH, 4, ghTileset, FALSE);
	}
}

// �w�背�x�����Z�[�u����֐�
bool Level::SaveLevel(int level)
{
	char fileName[MAX_PATH];

	snprintf(fileName, MAX_PATH, "%slevel_%03d.csv", LEVEL_DATA_PATH, level);

	std::ofstream ofs(fileName);
	if (!ofs)
	{
		return false;
	}
	
	// �X�e�[�W�f�[�^���Z�[�u
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
		{
			ofs << static_cast<int>(m_page2[i][j]) << ",";
		}
		ofs << std::endl;
	}

	//�t�@�C�������
	ofs.close();

	return true;
}

// �w�背�x�������[�h����֐�
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

	//�t�@�C�������
	ifs.close();

	// ���x����ݒ�
	m_levelNo = level;

	return false;
}

// �n�V�S���o������֐�
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

// �w��ʒu�̃����K�𕜌�����
void Level::SetFillBrick(int x, int y)
{
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		// ���g�p�̃��[�N�Ȃ�
		if (m_digBrick[i].timer == 0)
		{
			// �����K�̕�������ݒ�
			m_digBrick[i].position.x = x;
			m_digBrick[i].position.y = y;
			m_digBrick[i].timer = BRICK_FILL_FRAME;
			return;
		}
	}
}

