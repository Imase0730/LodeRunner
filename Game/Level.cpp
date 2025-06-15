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
	, m_levelId{ 0 }
	, m_guardCount{ 0 }
	, m_goldCount{ 0 }
	, m_invisibleLadderCount{ 0 }
	, m_playerPosition{ 0, 0 }
	, m_guardPosition{}
	, m_invisibleLadderPosition{}
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
	if (levelNo != m_levelId) LoadLevel(levelNo, mode);

	// �X�e�[�W�f�[�^�̏�����
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN+ 1; j++)
		{
			m_page1[i][j] = Tile::Type::Empty;
			m_page2[i][j] = Tile::Type::Empty;
		}
	}

	// ���[�h��ݒ�
	m_mode = mode;

	// �K�[�h�̐���������
	m_guardCount = 0;

	// ����̐���������
	m_goldCount = 0;

	// �B���n�V�S�̐���������
	m_invisibleLadderCount = 0;

	// ���[�h�����f�[�^����X�e�[�W�f�[�^���쐬
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
		{
			// �^�C���̎��
			Tile::Type type = m_loadData[i][j];

			// �v���C���[�Ȃ�ʒu���擾
			if (type == Tile::Type::Player) m_playerPosition = POINT{ j, i };

			// �K�[�h�Ȃ�
			if ((type == Tile::Type::Guard) && (m_guardCount < GUARD_MAX - 1))
			{
				// �K�[�h�̈ʒu���L��
				m_guardPosition[m_guardCount] = POINT{ j, i };
				// �K�[�h�̐������Z
				m_guardCount++;
			}

			// ����Ȃ����̐������Z
			if (type == Tile::Type::Gold) m_goldCount++;

			// �B���n�V�S�Ȃ�
			if ((type == Tile::Type::InvisibleLadder) && (m_invisibleLadderCount < INVISIBLE_LADDER_MAX - 1))
			{
				// �B���n�V�S�̈ʒu���L������
				m_invisibleLadderPosition[m_invisibleLadderCount] = POINT{ j, i };
				// �B���n�V�S�̐������Z
				m_invisibleLadderCount++;
			}

			// �X�e�[�W�f�[�^���쐬
			if ( (m_mode != Mode::GamePlay)
			  || ((type != Tile::Type::Player) && (type != Tile::Type::Guard) && (type != Tile::Type::InvisibleLadder))
			   )
			{
				m_page1[i][j] = m_page2[i][j] = type;
			}
		}
	}
}

// �X�V����
void Level::Update()
{
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
			int x = j * Tile::TILE_PIXEL_WIDTH;
			int y = i * Tile::TILE_PIXEL_HEIGHT;

			// �y�[�W�Q��`��Ώۂɂ���
			Tile::Type type = m_page2[i][j];

			// �Q�[���v���C�Ȃ�
			if (m_mode == Mode::GamePlay)
			{
				// 㩂̓����K�ŕ`��
				if (type == Tile::Type::Trap) type = Tile::Type::Blick;

				// Page2�F�����K�@Page1�F�󔒂̏ꍇ�A�@�������Ȃ̂ŋ󔒂�`��
				if ((m_page2[i][j] == Tile::Type::Blick) && (m_page1[i][j] == Tile::Type::Empty)) type = Tile::Type::Empty;
			}

			// �^�C���̊G�̈ʒu
			POINT pos = TILE_SPRITES[static_cast<int>(type)];
			// �^�C����`��
			DrawRectGraph(x, y
				, Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		}
	}

	// �X�e�[�W�̉����̕`��
	for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
	{
		DrawRectGraph(j * Tile::TILE_PIXEL_WIDTH, (MAX_GAME_ROW + 1) * Tile::TILE_PIXEL_HEIGHT
			, Tile::TILE_PIXEL_WIDTH * 2, Tile::TILE_PIXEL_HEIGHT * 4, Tile::TILE_PIXEL_WIDTH, 4, ghTileset, FALSE);
	}

	//// �X�e�[�W�̕`��
	//for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	//{
	//	for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
	//	{
	//		// �`��ʒu
	//		int x = j * Tile::TILE_PIXEL_WIDTH;
	//		int y = i * Tile::TILE_PIXEL_HEIGHT;

	//		// �y�[�W�Q�̃^�C����`��Ώۂɂ���
	//		Tile::Type tile = m_page1[i][j];

	//		// �^�C���̊G�̈ʒu
	//		POINT pos = TILE_SPRITES[static_cast<int>(tile)];
	//		// �^�C����`��
	//		DrawRectGraph(x, y
	//			, Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
	//			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
	//	}
	//}

}

// �w�背�x�����Z�[�u����֐�
bool Level::SaveLevel(int level) const
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
			m_loadData[i][j] = static_cast<Tile::Type>(std::stoi(item));
		}
	}

	//�t�@�C�������
	ifs.close();

	// ���x����ݒ�
	m_levelId = level;

	return false;
}

// �B���n�V�S���o������֐�
void Level::AppearLadder()
{
	for (int i = 0; i < m_invisibleLadderCount; i++)
	{
		POINT pos = m_invisibleLadderPosition[i];
		m_page2[pos.y][pos.x] = Tile::Type::Ladder;
	}
}

// Page2�̓��e��Page1�Ɏw��ʒu�̃^�C�����R�s�[����֐�
void Level::CopyPage2toPage1(int x, int y)
{
	// ���̏�Ȃ̂ŃR�s�[���Ȃ�
	if (m_page2[y][x] == Tile::Type::Blick)
	{
		m_page1[y][x] = Tile::Type::Empty;
	}
	else
	{
		m_page1[y][x] = m_page2[y][x];
	}
}

