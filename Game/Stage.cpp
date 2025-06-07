//--------------------------------------------------------------------------------------
// File: Stage.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Stage.h"
#include "Game/Game.h"
#include <fstream>
#include <sstream>

// �R���X�g���N�^
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

// �f�X�g���N�^
Stage::~Stage()
{
}

// ����������
void Stage::Initialize(int level, Mode mode)
{
	// ���x��������Ă���΃��x�������[�h����
	if (level != m_level) LoadLevel(level, mode);

	// �X�e�[�W�f�[�^�̏�����
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			m_stageData[i][j].SetTileType(Tile::TileType::Empty);
		}
	}

	// ���[�h��ݒ�
	m_mode = mode;

	// �K�[�h�̐���������
	m_guardCount = 0;

	// ����̐���������
	m_goldCount = 0;

	// ���[�h�����f�[�^����X�e�[�W�f�[�^���쐬
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			Tile::TileType type = m_loadData[i][j];

			// �v���C���[�Ȃ�ʒu���擾
			if (type == Tile::TileType::Player) m_playerPosition = POINT{ j, i };

			// �K�[�h�Ȃ�ʒu���擾
			if ((m_guardCount < GUARD_MAX - 1) && (type == Tile::TileType::Guard))
			{
				m_guardPosition[m_guardCount] = POINT{ j, i };
				m_guardCount++;
			}

			// ����Ȃ����̐������Z
			if (type == Tile::TileType::Gold) m_goldCount++;

			// �X�e�[�W�f�[�^���쐬
			if ( (m_mode != Mode::GamePlay)
			  || ((type != Tile::TileType::Player) && (type != Tile::TileType::Guard))
			   )
			{
				m_stageData[i][j].SetTileType(type);
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
void Stage::Update()
{
	// �@�����u���b�N�����ɖ߂�
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		// �����^�C�}�[���O�łȂ�
		if (m_digBrick[i].timer != 0)
		{
			// �����^�C�}�[�����Z����
			m_digBrick[i].timer--;

			// �������郌���K�̈ʒu
			int x = m_digBrick[i].position.x;
			int y = m_digBrick[i].position.y;

			// �����K�̕����A�j���[�V����
			if (m_digBrick[i].timer == BRICK_ANIME_TIME_FILL01) m_stageData[y][x].SetDigAnimationState(Tile::DigAnimationState::Fill01);
			if (m_digBrick[i].timer == BRICK_ANIME_TIME_FILL02) m_stageData[y][x].SetDigAnimationState(Tile::DigAnimationState::Fill02);
			if (m_digBrick[i].timer == 0) m_stageData[y][x].SetDigAnimationState(Tile::DigAnimationState::NotDigging);
			if (m_digBrick[i].timer == 0) m_stageData[y][x].SetTileType(Tile::TileType::Blick);
		}
	}

	// ���򂪑S�ĂȂ��Ȃ�����B��n�V�S���o��������
	if (m_goldCount == 0)
	{
		// ���x���o�����������Ȃ��悤�ɁA����̐��������Ă���
		m_goldCount--;
		AppearLadder();
	}

}

// �`�揈��
void Stage::Render(int ghTileset) const
{
	// 㩂������K�ɕύX���邽�߂̃^�C��
	Tile trapTile;
	trapTile.SetTileType(Tile::TileType::Blick);

	// �X�e�[�W�̕`��
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			// �`��ʒu
			int x = j * Tile::TILE_PIXEL_WIDTH;
			int y = i * Tile::TILE_PIXEL_HEIGHT;

			// �Q�[���v���C�Ȃ�
			if (m_mode == Mode::GamePlay)
			{
				// �B��n�V�S�͕\�����Ȃ�
				if (m_stageData[i][j].GetTileType() == Tile::TileType::InvisibleLadder) continue;

				// 㩂̓����K�ŕ\������
				if (m_stageData[i][j].GetTileType() == Tile::TileType::Trap)
				{
					trapTile.Render(x, y, ghTileset);
					continue;
				}
			}
			// �^�C����`��
			m_stageData[i][j].Render(x, y, ghTileset);
		}
	}

	// �X�e�[�W�̉����̕`��
	for (int j = 0; j < STAGE_WIDTH; j++)
	{
		DrawRectGraph(j * Tile::TILE_PIXEL_WIDTH, STAGE_HEIGHT * Tile::TILE_PIXEL_HEIGHT
			, Tile::TILE_PIXEL_WIDTH * 2, Tile::TILE_PIXEL_HEIGHT * 4, Tile::TILE_PIXEL_WIDTH, 4, ghTileset, FALSE);
	}
}

// �w��ꏊ�̃^�C����ݒ肷��֐�
void Stage::SetTileType(int x, int y, Tile::TileType type)
{
	// ���򂪂���ꍇ�͋���̐������炷
	if (m_stageData[y][x].GetTileType() == Tile::TileType::Gold) m_goldCount--;

	// �^�C����ݒ肷��
	m_stageData[y][x].SetTileType(type);
}

// �w��ꏊ�̃^�C�����擾����֐�
Tile::TileType Stage::GetTileType(int x, int y)
{
	if ( (x < 0) || (x >= Stage::STAGE_WIDTH)
	  || (y < 0) || (y >= Stage::STAGE_HEIGHT)
	   )
	{
		// �X�e�[�W�O�̏ꍇ�A�΂�Ԃ�
		return Tile::TileType::Stone;
	}
	return m_stageData[y][x].GetTileType();
}

// �w�背�x�����Z�[�u����֐�
bool Stage::SaveLevel(int level)
{
	char fileName[MAX_PATH];

	snprintf(fileName, MAX_PATH, "%sstage_%03d.csv", STAGE_DATA_PATH, level);

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
			Tile::TileType tileType = m_stageData[i][j].GetTileType();
			ofs << static_cast<int>(tileType) << ",";
		}
		ofs << std::endl;
	}

	//�t�@�C�������
	ofs.close();

	return true;
}

// �w�背�x�������[�h����֐�
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

	//�t�@�C�������
	ifs.close();

	// ���x����ݒ�
	m_level = level;

	return false;
}

// �n�V�S���o������֐�
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

// �w��ʒu�̃����K�𕜌�����
void Stage::SetFillBrick(int x, int y)
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

