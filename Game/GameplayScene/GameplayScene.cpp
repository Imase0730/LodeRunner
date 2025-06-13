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
	, m_level{ Level::Mode::GamePlay }
	, m_scoreString{ POINT{ Level::TILE_PIXEL_WIDTH * 0 , Game::INFOMATION_Y }, "SCORE" }
	, m_menString{ POINT{ Level::TILE_PIXEL_WIDTH * 13 , Game::INFOMATION_Y }, "MEN" }
	, m_levelString{ POINT{ Level::TILE_PIXEL_WIDTH * 20 , Game::INFOMATION_Y }, "LEVEL" }
	, m_scoreNumber{ POINT{ 5 * Level::TILE_PIXEL_WIDTH, Game::INFOMATION_Y }, 7 }
	, m_menNumber{ POINT{ 16 * Level::TILE_PIXEL_WIDTH, Game::INFOMATION_Y }, 3 }
	, m_levelNumber{ POINT{ 25 * Level::TILE_PIXEL_WIDTH, Game::INFOMATION_Y }, 3 }
	, m_score{ 0 }
	, m_men{ 0 }
	, m_levelId{ 0 }
	, m_player{ this, &m_level }
	, m_pGurad{}
	, m_guradPattern{ 0, 0, 0 }
	, m_guradPhase{ 0 }
	, m_guradNumber{ 0 }
{
	// �K�[�h�𐶐�
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		m_pGurad[i] = new Gurad(this, &m_level);
	}
}

// �f�X�g���N�^
GamePlayScene::~GamePlayScene()
{
	// �����������
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		delete m_pGurad[i];
	}
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
	m_levelId = 1;
	m_levelNumber.SetNumber(m_levelId);

	// �Q�[���X�^�[�g���̏�����
	GameInitialize();

	// ���C�v�I�[�v��
	m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Open);
}

// �X�V����
void GamePlayScene::Update(int keyCondition, int keyTrigger)
{
	// �X�V�Ԋu�𒲐�����
	if (++m_updateCounter < UPDATE_INTERVAL) return;
	m_updateCounter = 0;

	static int oldKey = 0;

	keyTrigger = ~oldKey & keyCondition;

	// ���C�v���쒆�Ȃ牽�����Ȃ�
	if (m_pGame->GetIrisWipe()->IsActive()) return;

	// ���C�v��������
	if (m_pGame->GetIrisWipe()->GetMode() == IrisWipe::Mode::Close)
	{
		// �Q�[����������
		GameInitialize();
		// ���C�v�I�[�v��
		m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Open);
	}

	// �X�e�[�W�̍X�V
	m_level.Update();

	// �v���C���[�̍X�V
	m_player.Update(keyCondition, keyTrigger);

	// �K�[�h�̍X�V
	GuradsUpdate();

	// �v���C���[�������Ă���Ȃ�
	if (m_player.IsAlive())
	{
		// �X�e�[�W�N���A�Ȃ烏�C�v�����
		if (IsLevelCleared()) m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Close);
	}
	else
	{
		// �c�@�������炷
		m_menNumber.SetNumber(--m_men);

		// �c�@�����O�Ȃ�
		if (m_men == 0)
		{
			// �^�C�g����
			m_pGame->RequestSceneChange(Game::SceneID::Title);
		}
		else
		{
			// ���C�v�N���[�Y
			m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Close);
		}
	}

	oldKey = keyCondition;
}

// �`�揈��
void GamePlayScene::Render(int ghTileset)
{
	// �X�e�[�W�̕`��
	m_level.Render(ghTileset);

	// �v���C���[�̕`��
	m_player.Render(ghTileset);

	// �K�[�h�̕`��
	for (int i = 0; i < m_level.GetGuardCount(); i++)
	{
		m_pGurad[i]->Render(ghTileset);
	}

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
	// �v���C���[�������Ă���
	if (m_player.IsAlive())
	{
		// ���̃��x����
		m_levelNumber.SetNumber(++m_levelId);
	}

	// �X�e�[�W�̃��[�h
	m_level.Initialize(m_levelId, Level::Mode::GamePlay);

	// �v���C���[�̏�����
	POINT pos = m_level.GetPlayerPosition();
	m_player.Initialize(POINT{ pos.x, pos.y }
	, POINT{ Level::TILE_CENTER_X, Level::TILE_CENTER_Y });

	//// ----- �K�[�h�̏����� ----- //

	// �K�[�h�̐l���ɉ����čs���\�l���̃e�[�u����ݒ肷��
	for (int i = 0; i < GUARD_PHASE_COUNT; i++)
	{
		m_guradPattern[i] = GUARD_PATTERNS_LIST[m_level.GetGuardCount() * GUARD_PHASE_COUNT + i];
	}

	// �K�[�h�̈ʒu��ݒ�
	for (int i = 0; i < m_level.GetGuardCount(); i++)
	{
		POINT pos = m_level.GetGuardPosition(i);
		m_pGurad[i]->Initialize(POINT{ pos.x, pos.y }
		, POINT{ Level::TILE_CENTER_X, Level::TILE_CENTER_Y });
	}
}

// ���x���N���A�����ׂ�֐�
bool GamePlayScene::IsLevelCleared()
{
	if ( (m_level.GetGoldCount() < 0)								// �B���n�V�S�o���ς�
	  && (m_player.GetTilePosition().y == 0)						// ��ԏ�̍s�H
	  && (m_player.GetAdjustPosition().y == Level::TILE_CENTER_Y)	// �^�C����̈ʒu�����S�H
	   )
	{
		return true;
	}
	return false;
}

// �K�[�h�̍X�V����
void GamePlayScene::GuradsUpdate()
{
	// �P�t���[���ɍs���ł���K�[�h�̐l�������[�v
	for (int i = 0; i < m_guradPattern[m_guradPhase]; i++)
	{
		// �K�[�h���A�N�e�B�u�Ȃ�X�V
		if (m_pGurad[m_guradNumber]->IsActive()) m_pGurad[m_guradNumber]->Update();
		// ���̍s������K�[�h�ԍ���
		m_guradNumber = (m_guradNumber + 1) % m_level.GetGuardCount();
	}
	// ���̍s���\�l����
	m_guradPhase = (m_guradPhase + 1) % GUARD_PHASE_COUNT;
}

// ���_�����Z����֐�
void GamePlayScene::AddScore(int score)
{
	m_score += score;
	m_scoreNumber.SetNumber(m_score);
}


