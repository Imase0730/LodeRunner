//--------------------------------------------------------------------------------------
// File: Tile.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Tile.h"
#include "Stage.h"
#include "Game/Game.h"

// �R���X�g���N�^
Tile::Tile()
	: m_tileType{ TileType::Empty }
{
}

// �f�X�g���N�^
Tile::~Tile()
{
}

// ����������
void Tile::Initialize()
{
}

// �X�V����
void Tile::Update()
{
}

// �`�揈��
void Tile::Render(int x, int y, int ghTileset) const
{
	// ��̏ꍇ�͕\�����Ȃ�
	if (m_tileType == TileType::Empty) return;

	// �^�C���̕`��
	DrawRectGraph(x, y
		, TILE_PIXEL_WIDTH * static_cast<int>(m_tileType), TILE_PIXEL_HEIGHT * 4
		, TILE_PIXEL_WIDTH, TILE_PIXEL_HEIGHT, ghTileset, TRUE);
}

// �^�C�����ړ��\�����ׂ�֐��i�㍶�E�j
bool Tile::IsMovableTileULR(TileType tileType)
{
	// �u���b�N�A�΁A㩂Ȃ�ړ��s��
	if ( (tileType == Tile::TileType::Blick)
	  || (tileType == Tile::TileType::Stone)
	  || (tileType == Tile::TileType::Trap)
	   )
	{
		return false;
	}
	return true;
}

// �^�C�����ړ��\�����ׂ�֐��i���j
bool Tile::IsMovableTileDown(TileType tileType)
{
	// �u���b�N�A�΂Ȃ�ړ��s��
	if ( (tileType == Tile::TileType::Blick)
	  || (tileType == Tile::TileType::Stone)
	   )
	{
		return false;
	}
	return true;
}

// �^�C���������\�����ׂ�֐��i�����j
bool Tile::IsMovableTileFall(TileType tileType)
{
	// �u���b�N�A�΁A�n�V�S�Ȃ�ړ���
	if ( (tileType == Tile::TileType::Blick)
	  || (tileType == Tile::TileType::Stone)
	  || (tileType == Tile::TileType::Ladder)
	   )
	{
		return false;
	}
	return true;
}


