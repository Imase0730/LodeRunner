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
		, TILE_WIDTH * static_cast<int>(m_tileType), TILE_HEIGHT * 4
		, TILE_WIDTH, TILE_HEIGHT, ghTileset, TRUE);
}

// �^�C�����ړ��\�����ׂ�֐�
bool Tile::IsMovableTile(TileType tileType)
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


