//--------------------------------------------------------------------------------------
// File: StageEditScene.cpp
//
//--------------------------------------------------------------------------------------
#include "StageEditScene.h"
#include "Game/Game.h"

// �R���X�g���N�^
StageEditScene::StageEditScene(Game* pGame)
	: m_pGame{ pGame }
	, m_stage{ Stage::Mode::StageEdit }
	, m_ghTileset{}
	, m_cursorEdit{ 0, 0 }
	, m_selectTile{ Tile::TileType::Blick }
	, m_blink{ BLINK_INTEVAL }
	, m_level{ 1 }
	, m_levelNumber{ POINT{ 25 * Tile::TILE_WIDTH, Screen::GAME_HEIGHT - Tile::TILE_HEIGHT }, 3 }
	, m_mode{ Mode::Tile }
{
}

// �f�X�g���N�^
StageEditScene::~StageEditScene()
{
}

// ����������
void StageEditScene::Initialize(int ghTileset)
{
	m_ghTileset = ghTileset;
	// ���x���̐ݒ�
	m_levelNumber.SetNumber(m_level);
}

// �X�V����
void StageEditScene::Update(int keyCondition, int keyTrigger)
{
	static int elapseFrame = 0;
	int keyRepeat = 0;

	m_blink.Update();

	// �ȈՃL�[���s�[�g
	if (keyCondition)
	{
		elapseFrame++;
		if (elapseFrame >= KEY_REPEAT_INTEVAL)
		{
			elapseFrame = 0;
			keyRepeat = keyCondition;
		}
	}

	// �X�y�[�X�L�[�Ń��[�h�ύX
	if (keyTrigger & PAD_INPUT_10)
	{
		switch (m_mode)
		{
		case Mode::Tile:
			m_mode = Mode::Save;
			break;
		case Mode::Save:
			m_mode = Mode::Load;
			break;
		case Mode::Load:
			m_mode = Mode::Tile;
			break;
		default:
			break;
		}
	}

	// �I�𒆂̃��[�h�ɂ�菈���𕪂���
	switch (m_mode)
	{
	case Mode::Tile:
		SelectTile(keyCondition, keyRepeat);
		break;
	case Mode::Save:
		Save(keyTrigger, keyRepeat);
		break;
	case Mode::Load:
		Load(keyTrigger, keyRepeat);
		break;
	default:
		break;
	}

}

// �`�揈��
void StageEditScene::Render()
{
	// �X�e�[�W�̕`��
	m_stage.Render(m_ghTileset);

	// �_��
	int col = static_cast<int>(100.0f + 155.0f * m_blink.GetBlinkRate());
	SetDrawBright(col, col, col);

	switch (m_mode)
	{
	case Mode::Tile:
		// �J�[�\���̕`��i�㕔�j
		DrawRectGraph(m_cursorEdit.x * Tile::TILE_WIDTH, m_cursorEdit.y * Tile::TILE_HEIGHT
			, Tile::TILE_WIDTH * 9, Tile::TILE_HEIGHT * 1
			, Tile::TILE_WIDTH, Tile::TILE_HEIGHT, m_ghTileset, TRUE);

		// �J�[�\���̕`��i�����j
		DrawRectGraph(Tile::TILE_WIDTH * (static_cast<int>(m_selectTile) - 1), Screen::GAME_HEIGHT - Tile::TILE_HEIGHT
			, Tile::TILE_WIDTH * 9, Tile::TILE_HEIGHT * 1
			, Tile::TILE_WIDTH, Tile::TILE_HEIGHT, m_ghTileset, TRUE);

		// LEVEL�̓_�ł��~�߂�
		SetDrawBright(255, 255, 255);
		break;
	case Mode::Save:
		// SAVE�̕`��
		DrawRectGraph(Tile::TILE_WIDTH * 10, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5
			, Tile::TILE_WIDTH * 0, Tile::TILE_HEIGHT * 3
			, Tile::TILE_WIDTH * 4, Tile::TILE_HEIGHT, m_ghTileset, TRUE);
		break;
	case Mode::Load:
		// LOAD�̕`��
		DrawRectGraph(Tile::TILE_WIDTH * 15, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5
			, Tile::TILE_WIDTH * 4, Tile::TILE_HEIGHT * 3
			, Tile::TILE_WIDTH * 4, Tile::TILE_HEIGHT, m_ghTileset, TRUE);
		break;
	default:
		break;
	}

	// ���x���̕\��
	m_levelNumber.Render(m_ghTileset);

	// �_�ł��~�߂�
	SetDrawBright(255, 255, 255);
}

// �I������
void StageEditScene::Finalize()
{
}

void StageEditScene::SelectTile(int keyCondition, int keyRepeat)
{
	// �㕔�J�[�\���̈ړ�
	if (keyRepeat & PAD_INPUT_UP)    m_cursorEdit.y--;
	if (keyRepeat & PAD_INPUT_DOWN)  m_cursorEdit.y++;
	if (keyRepeat & PAD_INPUT_LEFT)  m_cursorEdit.x--;
	if (keyRepeat & PAD_INPUT_RIGHT) m_cursorEdit.x++;

	// �㕔�J�[�\���̉�ʊO�`�F�b�N
	if (m_cursorEdit.x < 0) m_cursorEdit.x = 0;
	if (m_cursorEdit.x > Stage::STAGE_WIDTH - 1) m_cursorEdit.x = Stage::STAGE_WIDTH - 1;
	if (m_cursorEdit.y < 0) m_cursorEdit.y = 0;
	if (m_cursorEdit.y > Stage::STAGE_HEIGHT - 1) m_cursorEdit.y = Stage::STAGE_HEIGHT - 1;

	// Z�L�[�ŃJ�[�\���ʒu�Ɏw��^�C����u��
	if (keyCondition & PAD_INPUT_1)
	{
		m_stage.SetTileType(m_cursorEdit.x, m_cursorEdit.y, m_selectTile);
	}
	// X�L�[�ŃJ�[�\���ʒu�̃^�C��������
	if (keyCondition & PAD_INPUT_2)
	{
		m_stage.SetTileType(m_cursorEdit.x, m_cursorEdit.y, Tile::TileType::None);
	}
	// A�L�[��D�L�[�őI���^�C���̕ύX
	if (keyRepeat & PAD_INPUT_4)
	{
		if (m_selectTile != Tile::TileType::Blick)
		{
			m_selectTile = static_cast<Tile::TileType>((static_cast<int>(m_selectTile) - 1));
		}
	}
	if (keyRepeat & PAD_INPUT_6)
	{
		if (m_selectTile != Tile::TileType::Player)
		{
			m_selectTile = static_cast<Tile::TileType>((static_cast<int>(m_selectTile) + 1));
		}
	}
	// S�L�[�ŃJ�[�\���ʒu�̃^�C����I��
	if (keyCondition & PAD_INPUT_5)
	{
		Tile::TileType type = m_stage.GetTileType(m_cursorEdit.x, m_cursorEdit.y);
		if (type != Tile::TileType::None) m_selectTile = type;
	}
}

// �Z�[�u
void StageEditScene::Save(int keyTrigger, int keyRepeat)
{
	if (keyTrigger & PAD_INPUT_1)
	{
		m_stage.SaveLevel(m_level);
	}
	if (keyRepeat & PAD_INPUT_UP)
	{
		if (m_level < STAGE_MAX) m_level++;
	}
	if (keyRepeat & PAD_INPUT_DOWN)
	{
		if (m_level > 1) m_level--;
	}
	// ���x���̐ݒ�
	m_levelNumber.SetNumber(m_level);
}

// ���[�h
void StageEditScene::Load(int keyTrigger, int keyRepeat)
{
	if (keyTrigger & PAD_INPUT_1)
	{
		m_stage.LoadLevel(m_level);
	}
	if (keyRepeat & PAD_INPUT_UP)
	{
		if (m_level < STAGE_MAX) m_level++;
	}
	if (keyRepeat & PAD_INPUT_DOWN)
	{
		if (m_level > 1) m_level--;
	}
	// ���x���̐ݒ�
	m_levelNumber.SetNumber(m_level);
}


