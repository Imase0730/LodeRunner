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

	// �X�e�[�W�̃��[�h
	m_stage.LoadLevel(m_level, Stage::Mode::GamePlay);

	// �v���C���[�̏�����
	POINT pos = m_stage.GetPlayerPosition();
	m_player.Initialize( POINT{ pos.x, pos.y }
					   , POINT{ Tile::TILE_CENTER_X, Tile::TILE_CENTER_Y });

	// �A�C���X���C�v�I�[�v��
	m_pGame->GetIrisWipe()->Initialize(IrisWipe::Mode::Open);
	m_pGame->GetIrisWipe()->Start();
}

// �X�V����
void GamePlayScene::Update(int keyCondition, int keyTrigger)
{
	// �X�V�Ԋu�𒲐�����
	if (++m_updateCounter < UPDATE_INTERVAL) return;
	m_updateCounter = 0;

	static int oldKey = 0;

	// �X�e�[�W�̍X�V
	m_stage.Update();

	// �v���C���[�̍X�V
	m_player.Update(keyCondition, ~oldKey & keyCondition, &m_stage);

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


