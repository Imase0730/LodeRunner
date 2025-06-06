//--------------------------------------------------------------------------------------
// File: Tile.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Tile.h"

// コンストラクタ
Tile::Tile()
	: m_tileType{ TileType::Empty }
	, m_digAnimationState{ DigAnimationState::NotDigging }
{
}

// 描画処理
void Tile::Render(int x, int y, int ghTileset) const
{
	// タイルの絵の位置
	POINT pos = TILE_SPRITES[static_cast<int>(m_tileType)];

	// 掘っている最中のレンガなら
	if (m_digAnimationState != DigAnimationState::NotDigging)
	{
		// 掘っている最中のレンガの描画
		pos = DIG_BRICK_SPRITES[static_cast<int>(m_digAnimationState)];
	}

	// タイルの描画
	DrawRectGraph(x, y
		, TILE_PIXEL_WIDTH * pos.x, TILE_PIXEL_HEIGHT * pos.y
		, TILE_PIXEL_WIDTH, TILE_PIXEL_HEIGHT, ghTileset, TRUE);
}

// タイルタイプの設定関数
void Tile::SetTileType(TileType type)
{
	m_tileType = type;
	m_digAnimationState = DigAnimationState::NotDigging;
}

// 移動可能なタイルか調べる関数（上左右）
bool Tile::IsMovableTileULR(TileType tileType)
{
	// ブロック、石、罠なら移動不可
	if ( (tileType == Tile::TileType::Blick)
	  || (tileType == Tile::TileType::Stone)
	  || (tileType == Tile::TileType::Trap)
	   )
	{
		return false;
	}
	return true;
}

// 移動可能なタイルか調べる関数（下）
bool Tile::IsMovableTileDown(TileType tileType)
{
	// ブロック、石なら移動不可
	if ( (tileType == Tile::TileType::Blick)
	  || (tileType == Tile::TileType::Stone)
	   )
	{
		return false;
	}
	return true;
}

// 移動可能なタイルか調べる関数（落下）
bool Tile::IsMovableTileFall(TileType tileType)
{
	// ブロック、石、ハシゴなら移動可
	if ( (tileType == Tile::TileType::Blick)
	  || (tileType == Tile::TileType::Stone)
	  || (tileType == Tile::TileType::Ladder)
	   )
	{
		return false;
	}
	return true;
}


