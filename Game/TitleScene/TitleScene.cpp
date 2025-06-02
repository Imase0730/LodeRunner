//--------------------------------------------------------------------------------------
// File: TitleScene.cpp
//
//--------------------------------------------------------------------------------------
#include "TitleScene.h"
#include "Game/Game.h"

// �R���X�g���N�^
TitleScene::TitleScene(Game* pGame)
	: m_pGame{ pGame }
	, m_titleString{ TITLE_POSITION, "LODE RUNNER" }
	, m_gameString{ GAME_POSITION, "GAME" }
	, m_editString{ EDIT_POSITION, "EDIT" }
	, m_mode{ Mode::Game }
	, m_blink{ BLINK_INTEVAL }
{
}

// �f�X�g���N�^
TitleScene::~TitleScene()
{
}

// ����������
void TitleScene::Initialize()
{
	// �e�ϐ��̏�����
	m_mode = Mode::Game;
}

// �X�V����
void TitleScene::Update(int keyCondition, int keyTrigger)
{
	// �_�ł̍X�V
	m_blink.Update();

	// �㉺�L�[�Ń��[�h��I��
	if (keyTrigger & PAD_INPUT_UP) m_mode = Mode::Game;
	if (keyTrigger & PAD_INPUT_DOWN) m_mode = Mode::Edit;

	// Enter�L�[�Ō���
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		// �e�V�[����
		if (m_mode == Mode::Game)
		{
			m_pGame->RequestSceneChange(Game::SceneID::GamePlay);
		}
		else if (m_mode == Mode::Edit)
		{
			m_pGame->RequestSceneChange(Game::SceneID::StageEdit);
		}
	}
}

// �`�揈��
void TitleScene::Render(int ghTileset)
{
	// �^�C�g���̕\��
	m_titleString.Render(ghTileset);

	// GAME�̕\��
	m_gameString.Render(ghTileset);

	// EDIT�̕\��
	m_editString.Render(ghTileset);

	// ----- ��������_�� ----- //
	int col = static_cast<int>(100.0f + 155.0f * m_blink.GetBlinkRate());
	SetDrawBright(col, col, col);

	// �J�[�\���̕\��
	if (m_mode == Mode::Game)
	{
		DrawRectGraph(GAME_POSITION.x - 18, GAME_POSITION.y + 1
			, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT * 4, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
	}
	else
	{
		DrawRectGraph(EDIT_POSITION.x - 18, EDIT_POSITION.y + 1
			, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT * 4, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
	}

	// ----- �_�ł��~�߂� ----- //
	SetDrawBright(255, 255, 255);
}

// �I������
void TitleScene::Finalize()
{
}


