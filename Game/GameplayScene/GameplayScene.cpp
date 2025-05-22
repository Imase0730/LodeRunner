//--------------------------------------------------------------------------------------
// File: GamePlayScene.cpp
//
//--------------------------------------------------------------------------------------
#include "GamePlayScene.h"
#include "Game/Game.h"

// �R���X�g���N�^
GamePlayScene::GamePlayScene(Game* pGame)
	: m_pGame{ pGame }
	, m_stage{ Stage::Mode::GamePlay }
	, m_ghTileset{ -1 }
{
}

// �f�X�g���N�^
GamePlayScene::~GamePlayScene()
{
}

// ����������
void GamePlayScene::Initialize(int ghTileset)
{
	m_ghTileset = ghTileset;
}

// �X�V����
void GamePlayScene::Update(int keyCondition, int keyTrigger)
{
}

// �`�揈��
void GamePlayScene::Render()
{
	// �X�e�[�W�̕`��
	m_stage.Render(m_ghTileset);
}

// �I������
void GamePlayScene::Finalize()
{
}


