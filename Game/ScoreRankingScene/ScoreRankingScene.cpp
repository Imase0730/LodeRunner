//--------------------------------------------------------------------------------------
// File: ScoreRankingScene.cpp
//
//--------------------------------------------------------------------------------------
#include "ScoreRankingScene.h"
#include "Game/Game.h"

// �R���X�g���N�^
ScoreRankingScene::ScoreRankingScene(Game* pGame)
	: m_pGame{ pGame }
	, m_mode{ Mode::Game }
	, m_stringRenderer{ POINT{ 0, 0 }, "" }
	, m_numberRenderer{ POINT{ 0, 0 }, 2, false }
	, m_initialStringRenderer{ POINT{ 0, 0 }, "" }
	, m_levelNumberRenderer{ POINT{0,0}, 3 }
	, m_scoreNumberRenderer{ POINT{0,0}, 8 }
{
}

// �f�X�g���N�^
ScoreRankingScene::~ScoreRankingScene()
{
}

// ����������
void ScoreRankingScene::Initialize()
{
	m_pGame->LoadScore();
}

// �X�V����
void ScoreRankingScene::Update(int keyCondition, int keyTrigger)
{
	// Q�L�[�Ń^�C�g����
	if (keyTrigger & PAD_INPUT_7)
	{
		m_pGame->RequestSceneChange(Game::SceneID::Title);
		return;
	}
}

// �`�揈��
void ScoreRankingScene::Render(int ghTileset)
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
	for (int i = 0; i < Game::SCORE_ENTRY_MAX; i++)
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
		
		// �X�R�A���o�^����Ă����
		if (m_pGame->GetScore(i).level != 0)
		{
			// �C�j�V����
			m_initialStringRenderer.SetString(m_pGame->GetScore(i).initial.c_str());
			m_initialStringRenderer.SetPosition(POINT{ 7 * Tile::TILE_PIXEL_WIDTH, (5 + i) * Tile::TILE_PIXEL_HEIGHT });
			m_initialStringRenderer.Render(ghTileset);

			// ���x��
			m_levelNumberRenderer.SetNumber(m_pGame->GetScore(i).level);
			m_levelNumberRenderer.SetPosition(POINT{ 14 * Tile::TILE_PIXEL_WIDTH, (5 + i) * Tile::TILE_PIXEL_HEIGHT });
			m_levelNumberRenderer.Render(ghTileset);

			// �X�R�A
			m_scoreNumberRenderer.SetNumber(m_pGame->GetScore(i).score);
			m_scoreNumberRenderer.SetPosition(POINT{ 19 * Tile::TILE_PIXEL_WIDTH, (5 + i) * Tile::TILE_PIXEL_HEIGHT });
			m_scoreNumberRenderer.Render(ghTileset);
		}
	}
}

// �I������
void ScoreRankingScene::Finalize()
{
}



