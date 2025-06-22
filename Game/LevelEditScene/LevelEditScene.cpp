//--------------------------------------------------------------------------------------
// File: LevelEditScene.cpp
//
//--------------------------------------------------------------------------------------
#include "LevelEditScene.h"
#include "Game/Game.h"
#include "Game/KeyRepeat.h"

// �R���X�g���N�^
LevelEditScene::LevelEditScene(Game* pGame)
	: m_pGame{ pGame }
	, m_level{ Level::Mode::LevelEdit }
	, m_cursorEdit{ 0, 0 }
	, m_selectTile{ Tile::Type::Blick }
	, m_blink{ BLINK_INTEVAL }
	, m_levelId{ 0 }
	, m_mode{ Mode::SelectTile }
	, m_saveStringRenderer{ POINT{ Tile::TILE_PIXEL_WIDTH * 10 , Level::INFOMATION_Y }, "SAVE" }
	, m_loadStringRenderer{ POINT{ Tile::TILE_PIXEL_WIDTH * 15 , Level::INFOMATION_Y }, "LOAD" }
	, m_levelStringRenderer{ POINT{ Tile::TILE_PIXEL_WIDTH * 20 , Level::INFOMATION_Y }, "LEVEL" }
	, m_levelNumberRenderer{ POINT{ Tile::TILE_PIXEL_WIDTH * 25, Level::INFOMATION_Y }, 3 }
	, m_yesNoDialog{}
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
	m_selectTile = Tile::Type::Blick;
	m_levelId = 1;
	m_mode = Mode::SelectTile;

	// ���x���̐����̕\��
	m_levelNumberRenderer.SetNumber(m_levelId);

	// ���x���̏�����
	m_level.Initialize(m_levelId, Level::Mode::LevelEdit);
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
	int keyRepeat = KeyRepeat::GetKey(keyCondition);

	// �_�ł̍X�V
	m_blink.Update();

	// �X�y�[�X�L�[�Ń��[�h�ύX
	if (keyTrigger & PAD_INPUT_10)
	{
		// YesNo�_�C�A���O�\�����H
		if (m_yesNoDialog.IsVisivle())
		{
			// YesNo�_�C�A���O�����
			m_yesNoDialog.CloseDialog();
		}

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
	DrawRectGraph(0, Level::INFOMATION_Y
		, Tile::TILE_PIXEL_WIDTH * 1, Tile::TILE_PIXEL_HEIGHT * 4
		, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);

	// �e������̕`��
	if (m_mode != Mode::Save) m_saveStringRenderer.Render(ghTileset);			// SAVE
	if (m_mode != Mode::Load) m_loadStringRenderer.Render(ghTileset);			// LOAD
	m_levelStringRenderer.Render(ghTileset);									// LEVEL
	if (m_mode == Mode::SelectTile) m_levelNumberRenderer.Render(ghTileset);	// 000

	// ----- ��������_�� ----- //
	int col = static_cast<int>(100.0f + 155.0f * m_blink.GetBlinkRate());
	SetDrawBright(col, col, col);

	switch (m_mode)
	{
	case Mode::SelectTile:
		// �J�[�\���̕`��i�㕔�j
		DrawRectGraph(m_cursorEdit.x * Tile::TILE_PIXEL_WIDTH, m_cursorEdit.y * Tile::TILE_PIXEL_HEIGHT
			, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT * 3
			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);

		// �J�[�\���̕`��i�����j
		DrawRectGraph(Tile::TILE_PIXEL_WIDTH * (static_cast<int>(m_selectTile) - 1), Level::INFOMATION_Y
			, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT * 3
			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		break;
	case Mode::Save:
		m_saveStringRenderer.Render(ghTileset);	// SAVE
		m_levelNumberRenderer.Render(ghTileset);	// 000
		break;
	case Mode::Load:
		m_loadStringRenderer.Render(ghTileset);	// LOAD
		m_levelNumberRenderer.Render(ghTileset);	// 000
		break;
	default:
		break;
	}

	// �_�ł��~�߂�
	SetDrawBright(255, 255, 255);

	m_yesNoDialog.Render(ghTileset);
}

// �I������
void LevelEditScene::Finalize()
{
}

// �^�C���I��
void LevelEditScene::SelectTile(int keyCondition, int keyRepeat)
{
	// D�L�[�ƍ��E�L�[�őI���^�C���̕ύX
	if (keyCondition & PAD_INPUT_6)
	{
		if (keyRepeat & PAD_INPUT_LEFT)
		{
			if (m_selectTile != Tile::Type::Blick)
			{
				m_selectTile = static_cast<Tile::Type>(static_cast<int>(m_selectTile) - 1);
			}
		}
		if (keyRepeat & PAD_INPUT_RIGHT)
		{
			if (m_selectTile != Tile::Type::Player)
			{
				m_selectTile = static_cast<Tile::Type>(static_cast<int>(m_selectTile) + 1);
			}
		}
		return;
	}

	// W�L�[�őI���^�C���̕ύX�i���[�v�j
	if (keyRepeat & PAD_INPUT_8)
	{
		m_selectTile = static_cast<Tile::Type>((static_cast<int>(m_selectTile) + 1) % SELECT_TILE_MAX);
		if (m_selectTile == Tile::Type::Empty) m_selectTile = Tile::Type::Blick;
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
		m_level.SetTilePage2(m_cursorEdit.x, m_cursorEdit.y, Tile::Type::Empty);
	}
}

// �Z�[�u
void LevelEditScene::Save(int keyTrigger, int keyRepeat)
{
	// YesNo�_�C�A���O�\�����H
	if (m_yesNoDialog.IsVisivle())
	{
		// Yes���I�����ꂽ�H
		if ( (m_yesNoDialog.Update(keyTrigger) == true)
		  && (m_yesNoDialog.GetYesNo() == YesNoDialog::YesNo::Yes)
		   )
		{
				// �Z�[�u
				m_level.SaveLevel(m_levelId);
		}
		return;
	}

	// Z�L�[�ŃZ�[�u
	if (keyTrigger & PAD_INPUT_1)
	{
		// YesNo�_�C�A���O���N��
		m_yesNoDialog.StartDialog();
	}
	// ��L�[�܂���W�L�[�Ń��x�����Z
	if ((keyRepeat & PAD_INPUT_UP) || (keyRepeat & PAD_INPUT_8))
	{
		if (m_levelId < LEVEL_MAX) m_levelId++;
	}
	// ���L�[�܂���S�L�[�Ń��x�����Z
	if ((keyRepeat & PAD_INPUT_DOWN) || (keyRepeat & PAD_INPUT_5))
	{
		if (m_levelId > 1) m_levelId--;
	}
	// ���x���̐ݒ�
	m_levelNumberRenderer.SetNumber(m_levelId);
}

// ���[�h
void LevelEditScene::Load(int keyTrigger, int keyRepeat)
{
	// YesNo�_�C�A���O�\�����H
	if (m_yesNoDialog.IsVisivle())
	{
		// Yes���I�����ꂽ�H
		if ((m_yesNoDialog.Update(keyTrigger) == true)
			&& (m_yesNoDialog.GetYesNo() == YesNoDialog::YesNo::Yes)
			)
		{
			// ���[�h
			m_level.Initialize(m_levelId, Level::Mode::LevelEdit);
		}
		return;
	}

	// Z�L�[�Ń��[�h
	if (keyTrigger & PAD_INPUT_1)
	{
		// YesNo�_�C�A���O���N��
		m_yesNoDialog.StartDialog();
	}
	// ��L�[�܂���W�L�[�Ń��x�����Z
	if ((keyRepeat & PAD_INPUT_UP) || (keyRepeat & PAD_INPUT_8))
	{
		if (m_levelId < LEVEL_MAX) m_levelId++;
	}
	// ���L�[�܂���S�L�[�Ń��x�����Z
	if ((keyRepeat & PAD_INPUT_DOWN) || (keyRepeat & PAD_INPUT_5))
	{
		if (m_levelId > 1) m_levelId--;
	}
	// ���x���̐ݒ�
	m_levelNumberRenderer.SetNumber(m_levelId);
}

// �f�o�b�O����\������֐�
void LevelEditScene::DisplayDebugInformation(int offsetX, int offsetY) const
{
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 0, GetColor(255, 255, 255)
		, L"<< Keys >>");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 2, GetColor(255, 255, 255)
		, L"         ��");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 3, GetColor(255, 255, 255)
		, L"CURSOR ���@��");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 4, GetColor(255, 255, 255)
		, L"         ��");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 6, GetColor(255, 255, 255)
		, L"PUT TILE   A");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 8, GetColor(255, 255, 255)
		, L"DEL TILE   S");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 10, GetColor(255, 255, 255)
		, L"<Tile Select>");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 12, GetColor(255, 255, 255)
		, L"W or D + �� ��");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 16, GetColor(255, 255, 255)
		, L"MODE SpaceKey");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 18, GetColor(255, 255, 255)
		, L"SAVE LOAD  Z");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 21, GetColor(255, 255, 255)
		, L"QUIT Q");

}

