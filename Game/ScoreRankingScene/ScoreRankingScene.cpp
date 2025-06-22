//--------------------------------------------------------------------------------------
// File: ScoreRankingScene.cpp
//
//--------------------------------------------------------------------------------------
#include "ScoreRankingScene.h"
#include "Game/Game.h"
#include "Game/KeyRepeat.h"

// �R���X�g���N�^
ScoreRankingScene::ScoreRankingScene(Game* pGame)
	: m_pGame{ pGame }
	, m_mode{ Mode::Display }
	, m_stringRenderer{ POINT{ 0, 0 }, "" }
	, m_numberRenderer{ POINT{ 0, 0 }, 2, false }
	, m_initialStringRenderer{ POINT{ 0, 0 }, "" }
	, m_levelNumberRenderer{ POINT{0,0}, 3 }
	, m_scoreNumberRenderer{ POINT{0,0}, 8 }
	, m_entryIndex{ -1 }
	, m_blink{ BLINK_INTERVAL }
	, m_inputCharacter{ 'A' }
	, m_waitTimer{ 0 }
{
}

// �f�X�g���N�^
ScoreRankingScene::~ScoreRankingScene()
{
}

// ����������
void ScoreRankingScene::Initialize()
{
	// �\�����[�h
	m_mode = Mode::Display;

	// �o�^����X�R�A�����邩�H
	int score = m_pGame->GetEntryScore().score;
	if (score)
	{
		// �o�^����ʒu�����߂� 
		int index = GetScoreIndex(score);
		if (index < Game::SCORE_ENTRY_MAX)
		{
			// �o�^���[�h
			m_mode = Mode::Entry;

			// �o�^�ʒu
			m_entryIndex = index;

			// �o�^����ʒu���󂯂�
			for (int i = 0; i < Game::SCORE_ENTRY_MAX - 1 - index; i++)
			{
				int idx = Game::SCORE_ENTRY_MAX - 1 - i;
				m_pGame->SetScore(idx, m_pGame->GetScore(idx - 1));
			}

			// ����o�^����X�R�A��ݒ�
			m_pGame->SetScore(index, m_pGame->GetEntryScore());

			// ����o�^����X�R�A��������
			m_pGame->InitializeEntryScore();
		}
	}
}

// �X�V����
void ScoreRankingScene::Update(int keyCondition, int keyTrigger)
{
	// �_�ł̍X�V
	m_blink.Update();

	// ��ʐ؂�ւ����̃E�G�C�g
	if (m_waitTimer > 0)
	{
		m_waitTimer--;
		if (m_waitTimer == 0)
		{
			// �^�C�g����
			m_pGame->RequestSceneChange(Game::SceneID::Title);
		}
		return;
	}

	// �\�����[�h
	if (m_mode == Mode::Display)
	{
		// Q�L�[�Ń^�C�g����
		if (keyTrigger & PAD_INPUT_7)
		{
			m_pGame->RequestSceneChange(Game::SceneID::Title);
		}
	}
	else
	{
		// �o�^���[�h

		// �L�[���s�[�g���擾
		int keyRepeat = KeyRepeat::GetKey(keyCondition);

		// ���L�[�ŕ����I��
		if (keyRepeat & PAD_INPUT_LEFT)
		{
			if (m_inputCharacter != 'A') m_inputCharacter--;
		}

		// �E�L�[�ŕ����I��
		if (keyRepeat & PAD_INPUT_RIGHT)
		{
			if (m_inputCharacter != '[') m_inputCharacter++;
		}

		// Z�L�[�Ō���
		if (keyTrigger & PAD_INPUT_1)
		{
			Game::Score score = m_pGame->GetScore(m_entryIndex);
			score.initial += m_inputCharacter;
			m_inputCharacter = 'A';
			m_pGame->SetScore(m_entryIndex, score);

			// ���͏I���H
			if (score.initial.size() == 3)
			{
				// �X�R�A�̃Z�[�u
				m_pGame->SaveScore();
				// ��ʐ؂�ւ����̃E�G�C�g�l��ݒ�
				m_waitTimer = TRANSITION_DELAY_FRAMES;
			}
		}
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

		// �X�R�A�G���g���[�Ȃ�
		if (m_mode == Mode::Entry)
		{
			// �_�ł�����͒��̕���
			Game::Score score = m_pGame->GetScore(m_entryIndex);
			int size = score.initial.size();
			if ((m_blink.IsBlinkOn()) && (i == m_entryIndex) && (size < 3))
			{
				char str[]{ m_inputCharacter, '\0' };
				m_initialStringRenderer.SetString(str);
				m_initialStringRenderer.SetPosition(POINT{ (7 + size) * Tile::TILE_PIXEL_WIDTH, (5 + i) * Tile::TILE_PIXEL_HEIGHT });
				m_initialStringRenderer.Render(ghTileset);
			}
		}
	}
}

// �I������
void ScoreRankingScene::Finalize()
{
}

// �o�^����ʒu���擾����֐�
int ScoreRankingScene::GetScoreIndex(int score)
{
	int i{};

	for (i = 0; i < Game::SCORE_ENTRY_MAX; i++)
	{
		if (score > m_pGame->GetScore(i).score) return i;
	}
	
	return i;
}

// �f�o�b�O����\������֐�
void ScoreRankingScene::DisplayDebugInformation(int offsetX, int offsetY) const
{
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 0, GetColor(255, 255, 255)
		, L"<< Keys >>");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 2, GetColor(255, 255, 255)
		, L"QUIT   Q");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 5, GetColor(255, 255, 255)
		, L"<Score Entry>");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 7, GetColor(255, 255, 255)
		, L"��  ��");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 9, GetColor(255, 255, 255)
		, L"ENTER   Z");
}

