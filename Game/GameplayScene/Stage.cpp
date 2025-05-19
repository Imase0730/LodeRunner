//--------------------------------------------------------------------------------------
// File: Stage.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Stage.h"
#include "Game/Game.h"
#include "GameplayScene.h"

Stage::Stage(GameplayScene* scene)
	: m_pGameplayScene{ scene }
	, m_ghGame_bottom{}
	, m_ghEdit_bottom{}
{
	// �G�̃��[�h
	m_ghGame_bottom = LoadGraph(L"Resources/Textures/game_bottom.png");
	m_ghEdit_bottom = LoadGraph(L"Resources/Textures/edit_bottom.png");
}

Stage::~Stage()
{
	// �G�̃f�[�^������������폜
	DeleteGraph(m_ghGame_bottom);
	DeleteGraph(m_ghEdit_bottom);
}

void Stage::Initialize()
{
}

void Stage::Update()
{
}

void Stage::Render(int ghTileset) const
{
	// �X�e�[�W�̕`��
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			m_tileMap[i][j].Render(j * Tile::TILE_WIDTH, i * Tile::TILE_HEIGHT, ghTileset);
		}
	}

	// �����̕�����̕`��
	if (m_pGameplayScene->GetMode() == GameplayScene::Mode::GamePlay)
	{
		DrawGraph(0, Tile::TILE_HEIGHT * STAGE_HEIGHT, m_ghGame_bottom, TRUE);
	}
	else
	{
		DrawGraph(0, Tile::TILE_HEIGHT * STAGE_HEIGHT, m_ghEdit_bottom, TRUE);
	}


}

