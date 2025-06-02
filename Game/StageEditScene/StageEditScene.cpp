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
	, m_cursorEdit{ 0, 0 }
	, m_selectTile{ Tile::TileType::Blick }
	, m_blink{ BLINK_INTEVAL }
	, m_level{ 1 }
	, m_mode{ Mode::SelectTile }
	, m_saveString{ POINT{ Tile::TILE_PIXEL_WIDTH * 10 , Game::INFOMATION_Y }, "SAVE" }
	, m_loadString{ POINT{ Tile::TILE_PIXEL_WIDTH * 15 , Game::INFOMATION_Y }, "LOAD" }
	, m_levelString{ POINT{ Tile::TILE_PIXEL_WIDTH * 20 , Game::INFOMATION_Y }, "LEVEL" }
	, m_levelNumber{ POINT{ Tile::TILE_PIXEL_WIDTH * 25, Game::INFOMATION_Y }, 3 }
{

}

// �f�X�g���N�^
StageEditScene::~StageEditScene()
{
}

// ����������
void StageEditScene::Initialize()
{
	// �e�ϐ��̏�����
	m_cursorEdit = POINT{ 0, 0 };
	m_selectTile = Tile::TileType::Blick;
	m_level = 1;
	m_mode = Mode::SelectTile;

	// ���x���̐ݒ�
	m_levelNumber.SetNumber(m_level);

	// �X�e�[�W�̃��[�h
	m_stage.LoadLevel(m_level, Stage::Mode::StageEdit);
}

// �X�V����
void StageEditScene::Update(int keyCondition, int keyTrigger)
{
	// Q�L�[�Ń^�C�g����
	if (keyTrigger & PAD_INPUT_7)
	{
		m_pGame->RequestSceneChange(Game::SceneID::Title);
		return;
	}

	// �ȈՃL�[���s�[�g
	int keyRepeat = KeyRepeat(keyCondition);

	// �_�ł̍X�V
	m_blink.Update();

	// �X�y�[�X�L�[�Ń��[�h�ύX
	if (keyTrigger & PAD_INPUT_10)
	{
		switch (m_mode)
		{
		case Mode::SelectTile:
			m_mode = Mode::Save;
			break;
		case Mode::Save:
			m_mode = Mode::Load;
			break;
		case Mode::Load:
			m_mode = Mode::SelectTile;
			break;
		default:
			break;
		}
	}

	// �I�𒆂̃��[�h�ɂ�菈���𕪂���
	switch (m_mode)
	{
	case Mode::SelectTile:
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
void StageEditScene::Render(int ghTileset)
{
	// �X�e�[�W�̕`��
	m_stage.Render(ghTileset);

	// �I��p�^�C���̕`��
	DrawRectGraph(0, Game::INFOMATION_Y
		, Tile::TILE_PIXEL_WIDTH * 1, Tile::TILE_PIXEL_HEIGHT * 4
		, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);

	// �e������̕`��
	if (m_mode != Mode::Save) m_saveString.Render(ghTileset);			// SAVE
	if (m_mode != Mode::Load) m_loadString.Render(ghTileset);			// LOAD
	m_levelString.Render(ghTileset);									// LEVEL
	if (m_mode == Mode::SelectTile) m_levelNumber.Render(ghTileset);	// 000

	// ----- ��������_�� ----- //
	int col = static_cast<int>(100.0f + 155.0f * m_blink.GetBlinkRate());
	SetDrawBright(col, col, col);

	switch (m_mode)
	{
	case Mode::SelectTile:
		// �J�[�\���̕`��i�㕔�j
		DrawRectGraph(m_cursorEdit.x * Tile::TILE_PIXEL_WIDTH, m_cursorEdit.y * Tile::TILE_PIXEL_HEIGHT
			, Tile::TILE_PIXEL_WIDTH * 6, Tile::TILE_PIXEL_HEIGHT * 3
			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);

		// �J�[�\���̕`��i�����j
		DrawRectGraph(Tile::TILE_PIXEL_WIDTH * (static_cast<int>(m_selectTile) - 1), Game::INFOMATION_Y
			, Tile::TILE_PIXEL_WIDTH * 6, Tile::TILE_PIXEL_HEIGHT * 3
			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		break;
	case Mode::Save:
		m_saveString.Render(ghTileset);	// SAVE
		m_levelNumber.Render(ghTileset);	// 000
		break;
	case Mode::Load:
		m_loadString.Render(ghTileset);	// LOAD
		m_levelNumber.Render(ghTileset);	// 000
		break;
	default:
		break;
	}

	// �_�ł��~�߂�
	SetDrawBright(255, 255, 255);
}

// �I������
void StageEditScene::Finalize()
{
}

// �ȈՃL�[���s�[�g
int StageEditScene::KeyRepeat(int keyCondition)
{
	// �L�[�������ꂽ�t���[����
	static int elapseFrame = 0;
	// �P�t���[���O�̃L�[���
	static int oldKey = 0;

	int keyRepeat = 0;

	// �L�[��������Ă���
	if (keyCondition)
	{
		// �L�[�������ꂽ�u�Ԃ̓L�[���𔽉f����
		if (~oldKey & keyCondition)
		{
			keyRepeat = keyCondition;
			elapseFrame = 0;
		}
		// ���s�[�g�Ԋu�𒴂���
		if (++elapseFrame >= KEY_REPEAT_INTEVAL)
		{
			elapseFrame = 0;
			keyRepeat = keyCondition;
		}
	}

	oldKey = keyCondition;

	return keyRepeat;
}

// �^�C���I��
void StageEditScene::SelectTile(int keyCondition, int keyRepeat)
{
	// C�L�[�ƍ��E�L�[�őI���^�C���̕ύX
	if (keyCondition & PAD_INPUT_3)
	{
		if (keyRepeat & PAD_INPUT_LEFT)
		{
			if (m_selectTile != Tile::TileType::Blick)
			{
				m_selectTile = static_cast<Tile::TileType>((static_cast<int>(m_selectTile) - 1));
			}
		}
		if (keyRepeat & PAD_INPUT_RIGHT)
		{
			if (m_selectTile != Tile::TileType::Player)
			{
				m_selectTile = static_cast<Tile::TileType>((static_cast<int>(m_selectTile) + 1));
			}
		}
		return;
	}

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

	// A�L�[�ŃJ�[�\���ʒu�Ƀ^�C����u��
	if (keyCondition & PAD_INPUT_1)
	{
		m_stage.SetTileType(m_cursorEdit.x, m_cursorEdit.y, m_selectTile);
	}
	// S�L�[�ŃJ�[�\���ʒu�̃^�C��������
	if (keyCondition & PAD_INPUT_2)
	{
		m_stage.SetTileType(m_cursorEdit.x, m_cursorEdit.y, Tile::TileType::Empty);
	}
}

// �Z�[�u
void StageEditScene::Save(int keyTrigger, int keyRepeat)
{
	// Z�L�[�ŃZ�[�u
	if (keyTrigger & PAD_INPUT_1)
	{
		m_stage.SaveLevel(m_level);
	}
	// ��L�[�܂���W�L�[�Ń��x�����Z
	if ((keyRepeat & PAD_INPUT_UP) || (keyRepeat & PAD_INPUT_8))
	{
		if (m_level < STAGE_MAX) m_level++;
	}
	// ���L�[�܂���S�L�[�Ń��x�����Z
	if ((keyRepeat & PAD_INPUT_DOWN) || (keyRepeat & PAD_INPUT_5))
	{
		if (m_level > 1) m_level--;
	}
	// ���x���̐ݒ�
	m_levelNumber.SetNumber(m_level);
}

// ���[�h
void StageEditScene::Load(int keyTrigger, int keyRepeat)
{
	// Z�L�[�Ń��[�h
	if (keyTrigger & PAD_INPUT_1)
	{
		if (m_level != m_stage.GetLevel())
		{
			m_stage.LoadLevel(m_level, Stage::Mode::StageEdit);
		}
	}
	// ��L�[�܂���W�L�[�Ń��x�����Z
	if ((keyRepeat & PAD_INPUT_UP) || (keyRepeat & PAD_INPUT_8))
	{
		if (m_level < STAGE_MAX) m_level++;
	}
	// ���L�[�܂���S�L�[�Ń��x�����Z
	if ((keyRepeat & PAD_INPUT_DOWN) || (keyRepeat & PAD_INPUT_5))
	{
		if (m_level > 1) m_level--;
	}
	// ���x���̐ݒ�
	m_levelNumber.SetNumber(m_level);
}


