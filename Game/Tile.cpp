//--------------------------------------------------------------------------------------
// File: Tile.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Tile.h"

// �R���X�g���N�^
Tile::Tile()
	: m_tileType{ TileType::Empty }
	, m_digAnimationState{ DigAnimationState::NotDigging }
{
}

// �`�揈��
void Tile::Render(int x, int y, int ghTileset) const
{
	// �^�C���̊G�̈ʒu
	POINT pos = TILE_SPRITES[static_cast<int>(m_tileType)];

	// �@���Ă���Œ��̃����K�Ȃ�
	if (m_digAnimationState != DigAnimationState::NotDigging)
	{
		// �@���Ă���Œ��̃����K�̕`��
		pos = DIG_BRICK_SPRITES[static_cast<int>(m_digAnimationState)];
	}

	// �^�C���̕`��
	DrawRectGraph(x, y
		, TILE_PIXEL_WIDTH * pos.x, TILE_PIXEL_HEIGHT * pos.y
		, TILE_PIXEL_WIDTH, TILE_PIXEL_HEIGHT, ghTileset, TRUE);
}

// �^�C���^�C�v�̐ݒ�֐�
void Tile::SetTileType(TileType type)
{
	m_tileType = type;
	m_digAnimationState = DigAnimationState::NotDigging;
}

// �ړ��\�ȃ^�C�������ׂ�֐��i�㍶�E�j
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

// �ړ��\�ȃ^�C�������ׂ�֐��i���j
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

// �ړ��\�ȃ^�C�������ׂ�֐��i�����j
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


