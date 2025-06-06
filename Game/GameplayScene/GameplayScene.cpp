//--------------------------------------------------------------------------------------
// File: GamePlayScene.cpp
//
//--------------------------------------------------------------------------------------
#include "GamePlayScene.h"
#include "Game/Game.h"

// �R���X�g���N�^
GamePlayScene::GamePlayScene(Game* pGame)
	: m_pGame{ pGame }
	, m_updateCounter{ 0 }
	, m_stage{ Stage::Mode::GamePlay }
	, m_scoreString{ POINT{ Tile::TILE_PIXEL_WIDTH * 0 , Game::INFOMATION_Y }, "SCORE" }
	, m_menString{ POINT{ Tile::TILE_PIXEL_WIDTH * 13 , Game::INFOMATION_Y }, "MEN" }
	, m_levelString{ POINT{ Tile::TILE_PIXEL_WIDTH * 20 , Game::INFOMATION_Y }, "LEVEL" }
	, m_scoreNumber{ POINT{ 5 * Tile::TILE_PIXEL_WIDTH, Game::INFOMATION_Y }, 7 }
	, m_menNumber{ POINT{ 16 * Tile::TILE_PIXEL_WIDTH, Game::INFOMATION_Y }, 3 }
	, m_levelNumber{ POINT{ 25 * Tile::TILE_PIXEL_WIDTH, Game::INFOMATION_Y }, 3 }
	, m_score{ 0 }
	, m_men{ 0 }
	, m_level{ 0 }
	, m_player{}
{
}

// �f�X�g���N�^
GamePlayScene::~GamePlayScene()
{
}

// ����������
void GamePlayScene::Initialize()
{
	// �X�R�A�̏�����
	m_score = 0;
	m_scoreNumber.SetNumber(m_score);

	// �v���C���[�̐��̏�����
	m_men = MEN_COUNT;
	m_menNumber.SetNumber(m_men);

	// ���x���̏�����
	m_level = 1;
	m_levelNumber.SetNumber(m_level);

	// �Q�[���X�^�[�g���̏�����
	GameInitialize();
}

// �X�V����
void GamePlayScene::Update(int keyCondition, int keyTrigger)
{
	// �X�V�Ԋu�𒲐�����
	if (++m_updateCounter < UPDATE_INTERVAL) return;
	m_updateCounter = 0;

	static int oldKey = 0;

	// ���C�v���쒆
	if (m_pGame->GetIrisWipe()->IsActive()) return;

	// ���C�v��������
	if (m_pGame->GetIrisWipe()->GetMode() == IrisWipe::Mode::Close)
	{
		// �Q�[�������������ă��C�v�I�[�v��
		GameInitialize();
	}

	// �X�e�[�W�̍X�V
	m_stage.Update();

	// �v���C���[�̍X�V
	m_player.Update(keyCondition, ~oldKey & keyCondition, &m_stage);

	// �v���C���[�����񂾂�
	if (!m_player.IsAlive())
	{
		// �c�@�������炷
		m_men--;
		m_menNumber.SetNumber(m_men);

		// �c�@�����O�Ȃ�
		if (m_men == 0)
		{
			// �^�C�g����
			m_pGame->RequestSceneChange(Game::SceneID::Title);
		}
		else
		{
			// ���C�v�N���[�Y���Ă�蒼��
			m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Close);
		}
	}

	oldKey = keyCondition;
}

// �`�揈��
void GamePlayScene::Render(int ghTileset)
{
	// �X�e�[�W�̕`��
	m_stage.Render(ghTileset);

	// �v���C���[�̕`��
	m_player.Render(ghTileset);

	// �wSCORE�x�̕����̕\��
	m_scoreString.Render(ghTileset);

	// �wMEN�x�̕����̕\��
	m_menString.Render(ghTileset);

	// �wLEVEL�x�̕����̕\��
	m_levelString.Render(ghTileset);

	// �X�R�A�̕\��
	m_scoreNumber.Render(ghTileset);

	// �c�@���̕\��
	m_menNumber.Render(ghTileset);

	// ���x���̕\��
	m_levelNumber.Render(ghTileset);
}

// �I������
void GamePlayScene::Finalize()
{
}

// �Q�[���X�^�[�g���̏�����
void GamePlayScene::GameInitialize()
{
	// �X�e�[�W�̃��[�h
	m_stage.Initialize(m_level, Stage::Mode::GamePlay);

	// �v���C���[�̏�����
	POINT pos = m_stage.GetPlayerPosition();
	m_player.Initialize(POINT{ pos.x, pos.y }
	, POINT{ Tile::TILE_CENTER_X, Tile::TILE_CENTER_Y });

	// ���C�v�I�[�v��
	m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Open);
}


