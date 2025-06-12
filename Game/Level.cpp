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
	, m_digBrick{}
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

	// �B���n�V�S�̐���������
	m_invisibleLadderCount = 0;

	// ���[�h�����f�[�^����X�e�[�W�f�[�^���쐬
	for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	{
		for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
		{
			// �^�C���̎��
			Tile tile = m_loadData[i][j];

			// �v���C���[�Ȃ�ʒu���擾
			if (tile == Tile::Player) m_playerPosition = POINT{ j, i };

			// �K�[�h�Ȃ�
			if ((tile == Tile::Guard) && (m_guardCount < GUARD_MAX - 1))
			{
				// �K�[�h�̈ʒu���L��
				m_guardPosition[m_guardCount] = POINT{ j, i };
				// �K�[�h�̐������Z
				m_guardCount++;
			}

			// ����Ȃ����̐������Z
			if (tile == Tile::Gold) m_goldCount++;

			// �B���n�V�S�Ȃ�
			if ((tile == Tile::InvisibleLadder) && (m_invisibleLadderCount < INVISIBLE_LADDER_MAX - 1))
			{
				// �B���n�V�S�̈ʒu���L������
				m_invisibleLadderPosition[m_invisibleLadderCount] = POINT{ j, i };
				// �B���n�V�S�̐������Z
				m_invisibleLadderCount++;
			}

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
	// �@�����u���b�N�����ɖ߂�
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		// �����^�C�}�[���O�łȂ�
		if (m_digBrick[i].timer != 0)
		{
			// �����^�C�}�[�����Z����
			m_digBrick[i].timer--;

			// �����K�ɖ߂�
			if (m_digBrick[i].timer == 0)
			{
				m_page1[m_digBrick[i].position.y][m_digBrick[i].position.x] = Tile::Blick;
			}
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

			// �y�[�W�Q�̃^�C����`��Ώۂɂ���
			Tile tile = m_page2[i][j];

			// �Q�[���v���C�Ȃ�
			if (m_mode == Mode::GamePlay)
			{
				// 㩂̓����K�ŕ\������
				if (m_page2[i][j] == Tile::Trap) tile = Tile::Blick;

				// �v���C���[�������J�����ꏊ�̏ꍇ
				if ((m_page2[i][j] == Tile::Blick) && (m_page1[i][j] == Tile::Empty))
				{
					// �����K�̕\���͂��Ȃ�
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

	// �������̃����K�̕`��
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		// �����^�C�}�[���O�łȂ�
		if (m_digBrick[i].timer)
		{
			// �@���Ă��郌���K�̊G�̈ʒu
			POINT pos = m_digBrick[i].position;
			if (m_digBrick[i].timer <= BRICK_ANIME_TIME_FILL02)
			{
				// �������̃����K�Q
				POINT spritePos = FILL_BRICK_SPRITES[static_cast<int>(FillAnimationState::Fill02)];
				DrawRectGraph(pos.x * TILE_PIXEL_WIDTH, pos.y * TILE_PIXEL_HEIGHT
					, TILE_PIXEL_WIDTH * spritePos.x, TILE_PIXEL_HEIGHT * spritePos.y
					, TILE_PIXEL_WIDTH, TILE_PIXEL_HEIGHT, ghTileset, TRUE);
			}
			else if (m_digBrick[i].timer <= BRICK_ANIME_TIME_FILL01)
			{
				// �������̃����K�P
				POINT spritePos = FILL_BRICK_SPRITES[static_cast<int>(FillAnimationState::Fill01)];
				DrawRectGraph(pos.x * TILE_PIXEL_WIDTH, pos.y * TILE_PIXEL_HEIGHT
					, TILE_PIXEL_WIDTH * spritePos.x, TILE_PIXEL_HEIGHT * spritePos.y
					, TILE_PIXEL_WIDTH, TILE_PIXEL_HEIGHT, ghTileset, TRUE);
			}
		}
	}

	// �X�e�[�W�̉����̕`��
	for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
	{
		DrawRectGraph(j * TILE_PIXEL_WIDTH, (MAX_GAME_ROW + 1) * TILE_PIXEL_HEIGHT
			, TILE_PIXEL_WIDTH * 2, TILE_PIXEL_HEIGHT * 4, TILE_PIXEL_WIDTH, 4, ghTileset, FALSE);
	}

	//// �X�e�[�W�̕`��
	//for (int i = 0; i < MAX_GAME_ROW + 1; i++)
	//{
	//	for (int j = 0; j < MAX_GAME_COLMUN + 1; j++)
	//	{
	//		// �`��ʒu
	//		int x = j * TILE_PIXEL_WIDTH;
	//		int y = i * TILE_PIXEL_HEIGHT;

	//		// �y�[�W�Q�̃^�C����`��Ώۂɂ���
	//		Tile tile = m_page1[i][j];

	//		// �^�C���̊G�̈ʒu
	//		POINT pos = TILE_SPRITES[static_cast<int>(tile)];
	//		// �^�C����`��
	//		DrawRectGraph(x, y
	//			, TILE_PIXEL_WIDTH * pos.x, TILE_PIXEL_HEIGHT * pos.y
	//			, TILE_PIXEL_WIDTH, TILE_PIXEL_HEIGHT, ghTileset, TRUE);
	//	}
	//}

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
	m_levelId = level;

	return false;
}

// �B���n�V�S���o������֐�
void Level::AppearLadder()
{
	for (int i = 0; i < m_invisibleLadderCount; i++)
	{
		POINT pos = m_invisibleLadderPosition[i];
		m_page2[pos.y][pos.x] = Tile::Ladder;
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

// �ړ��\�ȃ^�C�������ׂ�֐��i�㍶�E�j
bool Level::IsMovableTileULR(Level::Tile tile)
{
	// �u���b�N�A�΁A㩂Ȃ�ړ��s��
	if ((tile == Level::Tile::Blick)
		|| (tile == Level::Tile::Stone)
		|| (tile == Level::Tile::Trap)
		)
	{
		return false;
	}
	return true;
}

// �ړ��\�ȃ^�C�������ׂ�֐��i���j
bool Level::IsMovableTileDown(Level::Tile tile)
{
	// �u���b�N�A�΂Ȃ�ړ��s��
	if ((tile == Level::Tile::Blick)
		|| (tile == Level::Tile::Stone)
		)
	{
		return false;
	}
	return true;
}

