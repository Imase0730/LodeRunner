//--------------------------------------------------------------------------------------
// File: TitleScene.cpp
//
//--------------------------------------------------------------------------------------
#include "TitleScene.h"
#include "Game/Game.h"
#include "Game/Tile.h"

// �R���X�g���N�^
TitleScene::TitleScene(Game* pGame)
	: m_pGame{ pGame }
	, m_titleStringRenderer{ TITLE_POSITION, "LODE RUNNER" }
	, m_gameStringRenderer{ GAME_POSITION, "GAME" }
	, m_editStringRenderer{ EDIT_POSITION, "EDIT" }
	, m_scoreStringRenderer{ SCORE_POSITION, "SCORE" }
	, m_mode{ Mode::Game }
	, m_blink{ BLINK_INTERVAL }
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
	if (keyTrigger & PAD_INPUT_UP)
	{
		switch (m_mode)
		{
		case TitleScene::Mode::Game:
			m_mode = Mode::Score;
			break;
		case TitleScene::Mode::Edit:
			m_mode = Mode::Game;
			break;
		case TitleScene::Mode::Score:
			m_mode = Mode::Edit;
			break;
		default:
			break;
		}
	}
	if (keyTrigger & PAD_INPUT_DOWN)
	{
		switch (m_mode)
		{
		case TitleScene::Mode::Game:
			m_mode = Mode::Edit;
			break;
		case TitleScene::Mode::Edit:
			m_mode = Mode::Score;
			break;
		case TitleScene::Mode::Score:
			m_mode = Mode::Game;
			break;
		default:
			break;
		}
	}

	// Z�L�[�Ō���
	if (keyTrigger & PAD_INPUT_1)
	{
		// �e�V�[����
		switch (m_mode)
		{
		case TitleScene::Mode::Game:
			m_pGame->RequestSceneChange(Game::SceneID::GamePlay);
			break;
		case TitleScene::Mode::Edit:
			m_pGame->RequestSceneChange(Game::SceneID::LevelEdit);
			break;
		case TitleScene::Mode::Score:
			m_pGame->RequestSceneChange(Game::SceneID::ScoreRanking);
			break;
		default:
			break;
		}
	}
}

// �`�揈��
void TitleScene::Render(int ghTileset)
{
	// �^�C�g���̕\��
	m_titleStringRenderer.Render(ghTileset);

	// GAME�̕\��
	m_gameStringRenderer.Render(ghTileset);

	// EDIT�̕\��
	m_editStringRenderer.Render(ghTileset);

	// SCORE�̕\��
	m_scoreStringRenderer.Render(ghTileset);

	// ----- ��������_�� ----- //
	int col = static_cast<int>(100.0f + 155.0f * m_blink.GetBlinkRate());
	SetDrawBright(col, col, col);

	// �J�[�\���̕\��
	switch (m_mode)
	{
	case TitleScene::Mode::Game:
		DrawRectGraph(GAME_POSITION.x - 18, GAME_POSITION.y + 1
			, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT * 4, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		break;
	case TitleScene::Mode::Edit:
		DrawRectGraph(EDIT_POSITION.x - 18, EDIT_POSITION.y + 1
			, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT * 4, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		break;
	case TitleScene::Mode::Score:
		DrawRectGraph(SCORE_POSITION.x - 18, SCORE_POSITION.y + 1
			, Tile::TILE_PIXEL_WIDTH * 9, Tile::TILE_PIXEL_HEIGHT * 4, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		break;
	default:
		break;
	}

	// ----- �_�ł��~�߂� ----- //
	SetDrawBright(255, 255, 255);
}

// �I������
void TitleScene::Finalize()
{
}


