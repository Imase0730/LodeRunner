//--------------------------------------------------------------------------------------
// File: HighScoresScene.cpp
//
//--------------------------------------------------------------------------------------
#include "HighScoresScene.h"
#include "Game/Game.h"

// �R���X�g���N�^
HighScoresScene::HighScoresScene(Game* pGame)
	: m_pGame{ pGame }
	, m_mode{ Mode::Game }
	, m_stringRenderer{ POINT{ 0, 0 },"" }
	, m_numberRenderer{ POINT{ 0, 0 }, 2, false }
{
}

// �f�X�g���N�^
HighScoresScene::~HighScoresScene()
{
}

// ����������
void HighScoresScene::Initialize()
{

}

// �X�V����
void HighScoresScene::Update(int keyCondition, int keyTrigger)
{
	// Q�L�[�Ń^�C�g����
	if (keyTrigger & PAD_INPUT_7)
	{
		m_pGame->RequestSceneChange(Game::SceneID::Title);
		return;
	}
}

// �`�揈��
void HighScoresScene::Render(int ghTileset)
{
	// �^�C�g��
	m_stringRenderer.SetString("LODE RUNNER HIGH SCORES");
	m_stringRenderer.SetPosition(POINT{ 4 * Tile::TILE_PIXEL_WIDTH, 0 });
	m_stringRenderer.Render(ghTileset);

	m_stringRenderer.SetString("INITIALS LEVEL  SCORE");
	m_stringRenderer.SetPosition(POINT{ 4 * Tile::TILE_PIXEL_WIDTH, 3 * Tile::TILE_PIXEL_HEIGHT });
	m_stringRenderer.Render(ghTileset);

	// ���̐�
	for (int i = 0; i < 23; i++)
	{
		if ((i != 8) && (i != 14))
		{
			DrawRectGraph(4 * Tile::TILE_PIXEL_WIDTH + i * Tile::TILE_PIXEL_WIDTH, 4 * Tile::TILE_PIXEL_HEIGHT
				, Tile::TILE_PIXEL_WIDTH * 7, Tile::TILE_PIXEL_HEIGHT * 3
				, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT
				, ghTileset, TRUE);
		}
	}

	// �X�R�A
	for (int i = 0; i < 10; i++)
	{
		// ����
		m_numberRenderer.SetNumber(i + 1);
		m_numberRenderer.SetPosition(POINT{ 0, (5 + i) * Tile::TILE_PIXEL_HEIGHT });
		m_numberRenderer.Render(ghTileset);

		// �h�b�g
		DrawRectGraph(2 * Tile::TILE_PIXEL_WIDTH, (5 + i) * Tile::TILE_PIXEL_HEIGHT
			, Tile::TILE_PIXEL_WIDTH * 6, Tile::TILE_PIXEL_HEIGHT * 3
			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT
			, ghTileset, TRUE);
		
	}
}

// �I������
void HighScoresScene::Finalize()
{
}


