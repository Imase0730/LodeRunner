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
	, m_scoreString{ POINT{ Tile::TILE_WIDTH * 0 , Game::INFOMATION_Y }, "SCORE" }
	, m_menString{ POINT{ Tile::TILE_WIDTH * 13 , Game::INFOMATION_Y }, "MEN" }
	, m_levelString{ POINT{ Tile::TILE_WIDTH * 20 , Game::INFOMATION_Y }, "LEVEL" }
	, m_scoreNumber{ POINT{ 5 * Tile::TILE_WIDTH, Screen::GAME_HEIGHT - Tile::TILE_HEIGHT }, 7 }
	, m_menNumber{ POINT{ 16 * Tile::TILE_WIDTH, Screen::GAME_HEIGHT - Tile::TILE_HEIGHT }, 3 }
	, m_levelNumber{ POINT{ 25 * Tile::TILE_WIDTH, Screen::GAME_HEIGHT - Tile::TILE_HEIGHT }, 3 }
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

	// �X�R�A�̕\��
	m_scoreString.Render(m_ghTileset);

	// �c�@���̕\��
	m_menString.Render(m_ghTileset);

	// ���x���̕\��
	m_levelString.Render(m_ghTileset);

	// �X�R�A�̕\��
	m_scoreNumber.Render(m_ghTileset);

	// �c�@���̕\��
	m_menNumber.Render(m_ghTileset);

	// ���x���̕\��
	m_levelNumber.Render(m_ghTileset);
}

// �I������
void GamePlayScene::Finalize()
{
}


