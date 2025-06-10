//--------------------------------------------------------------------------------------
// File: LevelEditScene.cpp
//
//--------------------------------------------------------------------------------------
#include "LevelEditScene.h"
#include "Game/Game.h"

// �R���X�g���N�^
LevelEditScene::LevelEditScene(Game* pGame)
	: m_pGame{ pGame }
	, m_level{ Level::Mode::LevelEdit }
	, m_cursorEdit{ 0, 0 }
	, m_selectTile{ Level::Tile::Blick }
	, m_blink{ BLINK_INTEVAL }
	, m_levelNo{ 0 }
	, m_mode{ Mode::SelectTile }
	, m_saveString{ POINT{ Level::TILE_PIXEL_WIDTH * 10 , Game::INFOMATION_Y }, "SAVE" }
	, m_loadString{ POINT{ Level::TILE_PIXEL_WIDTH * 15 , Game::INFOMATION_Y }, "LOAD" }
	, m_levelString{ POINT{ Level::TILE_PIXEL_WIDTH * 20 , Game::INFOMATION_Y }, "LEVEL" }
	, m_levelNumber{ POINT{ Level::TILE_PIXEL_WIDTH * 25, Game::INFOMATION_Y }, 3 }
{

}

// �f�X�g���N�^
LevelEditScene::~LevelEditScene()
{
}

// ����������
void LevelEditScene::Initialize()
{
	// �e�ϐ��̏�����
	m_cursorEdit = POINT{ 0, 0 };
	m_selectTile = Level::Tile::Blick;
	m_levelNo = 1;
	m_mode = Mode::SelectTile;

	// ���x���̐����̕\��
	m_levelNumber.SetNumber(m_levelNo);

	// ���x���̏�����
	m_level.Initialize(m_levelNo, Level::Mode::LevelEdit);
}

// �X�V����
void LevelEditScene::Update(int keyCondition, int keyTrigger)
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
void LevelEditScene::Render(int ghTileset)
{
	// �X�e�[�W�̕`��
	m_level.Render(ghTileset);

	// �I��p�^�C���̕`��
	DrawRectGraph(0, Game::INFOMATION_Y
		, Level::TILE_PIXEL_WIDTH * 1, Level::TILE_PIXEL_HEIGHT * 4
		, Level::TILE_PIXEL_WIDTH * 9, Level::TILE_PIXEL_HEIGHT, ghTileset, TRUE);

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
		DrawRectGraph(m_cursorEdit.x * Level::TILE_PIXEL_WIDTH, m_cursorEdit.y * Level::TILE_PIXEL_HEIGHT
			, Level::TILE_PIXEL_WIDTH * 6, Level::TILE_PIXEL_HEIGHT * 3
			, Level::TILE_PIXEL_WIDTH, Level::TILE_PIXEL_HEIGHT, ghTileset, TRUE);

		// �J�[�\���̕`��i�����j
		DrawRectGraph(Level::TILE_PIXEL_WIDTH * (static_cast<int>(m_selectTile) - 1), Game::INFOMATION_Y
			, Level::TILE_PIXEL_WIDTH * 6, Level::TILE_PIXEL_HEIGHT * 3
			, Level::TILE_PIXEL_WIDTH, Level::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
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
void LevelEditScene::Finalize()
{
}

// �ȈՃL�[���s�[�g
int LevelEditScene::KeyRepeat(int keyCondition)
{
	// �L�[�������ꂽ�t���[����
	static int elapseFrame = 0;
	// �P�t���[���O�̃L�[���
	static int oldKey = 0;

	int keyTrigger = ~oldKey & keyCondition;
	int keyRepeat = 0;

	// �L�[��������Ă���
	if (keyCondition)
	{
		// �L�[�������ꂽ�u�Ԃ̓L�[���𔽉f����
		if (keyTrigger)
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
void LevelEditScene::SelectTile(int keyCondition, int keyRepeat)
{
	// D�L�[�ƍ��E�L�[�őI���^�C���̕ύX
	if (keyCondition & PAD_INPUT_6)
	{
		if (keyRepeat & PAD_INPUT_LEFT)
		{
			if (m_selectTile != Level::Tile::Blick)
			{
				m_selectTile = static_cast<Level::Tile>(static_cast<int>(m_selectTile) - 1);
			}
		}
		if (keyRepeat & PAD_INPUT_RIGHT)
		{
			if (m_selectTile != Level::Tile::Player)
			{
				m_selectTile = static_cast<Level::Tile>(static_cast<int>(m_selectTile) + 1);
			}
		}
		return;
	}

	// W�L�[�őI���^�C���̕ύX�i���[�v�j
	if (keyRepeat & PAD_INPUT_8)
	{
		m_selectTile = static_cast<Level::Tile>((static_cast<int>(m_selectTile) + 1) % SELECT_TILE_MAX);
		if (m_selectTile == Level::Tile::Empty) m_selectTile = Level::Tile::Blick;
	}

	// �㕔�J�[�\���̈ړ�
	if (keyRepeat & PAD_INPUT_UP)    m_cursorEdit.y--;
	if (keyRepeat & PAD_INPUT_DOWN)  m_cursorEdit.y++;
	if (keyRepeat & PAD_INPUT_LEFT)  m_cursorEdit.x--;
	if (keyRepeat & PAD_INPUT_RIGHT) m_cursorEdit.x++;

	// �㕔�J�[�\���̉�ʊO�`�F�b�N
	if (m_cursorEdit.x < 0) m_cursorEdit.x = 0;
	if (m_cursorEdit.x > Level::MAX_GAME_COLMUN) m_cursorEdit.x = Level::MAX_GAME_COLMUN;
	if (m_cursorEdit.y < 0) m_cursorEdit.y = 0;
	if (m_cursorEdit.y > Level::MAX_GAME_ROW) m_cursorEdit.y = Level::MAX_GAME_ROW;

	// A�L�[�ŃJ�[�\���ʒu�Ƀ^�C����u��
	if (keyCondition & PAD_INPUT_4)
	{
		m_level.SetTilePage2(m_cursorEdit.x, m_cursorEdit.y, m_selectTile);
	}
	// S�L�[�ŃJ�[�\���ʒu�̃^�C��������
	if (keyCondition & PAD_INPUT_5)
	{
		m_level.SetTilePage2(m_cursorEdit.x, m_cursorEdit.y, Level::Tile::Empty);
	}
}

// �Z�[�u
void LevelEditScene::Save(int keyTrigger, int keyRepeat)
{
	// Z�L�[�ŃZ�[�u
	if (keyTrigger & PAD_INPUT_1)
	{
		m_level.SaveLevel(m_levelNo);
	}
	// ��L�[�܂���W�L�[�Ń��x�����Z
	if ((keyRepeat & PAD_INPUT_UP) || (keyRepeat & PAD_INPUT_8))
	{
		if (m_levelNo < LEVEL_MAX) m_levelNo++;
	}
	// ���L�[�܂���S�L�[�Ń��x�����Z
	if ((keyRepeat & PAD_INPUT_DOWN) || (keyRepeat & PAD_INPUT_5))
	{
		if (m_levelNo > 1) m_levelNo--;
	}
	// ���x���̐ݒ�
	m_levelNumber.SetNumber(m_levelNo);
}

// ���[�h
void LevelEditScene::Load(int keyTrigger, int keyRepeat)
{
	// Z�L�[�Ń��[�h
	if (keyTrigger & PAD_INPUT_1)
	{
		if (m_levelNo != m_level.GetLevel())
		{
			m_level.Initialize(m_levelNo, Level::Mode::LevelEdit);
		}
	}
	// ��L�[�܂���W�L�[�Ń��x�����Z
	if ((keyRepeat & PAD_INPUT_UP) || (keyRepeat & PAD_INPUT_8))
	{
		if (m_levelNo < LEVEL_MAX) m_levelNo++;
	}
	// ���L�[�܂���S�L�[�Ń��x�����Z
	if ((keyRepeat & PAD_INPUT_DOWN) || (keyRepeat & PAD_INPUT_5))
	{
		if (m_levelNo > 1) m_levelNo--;
	}
	// ���x���̐ݒ�
	m_levelNumber.SetNumber(m_levelNo);
}


