//--------------------------------------------------------------------------------------
// File: Tile.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Tile.h"
#include "Stage.h"
#include "Game/Game.h"

// コンストラクタ
Tile::Tile()
	: m_tileType{ TileType::Empty }
	, m_digAnimationState{ DigAnimationState::NotDigging }
{
}

// デストラクタ
Tile::~Tile()
{
}

// 初期化処理
void Tile::Initialize()
{
}

// 更新処理
void Tile::Update()
{
}

// 描画処理
void Tile::Render(int x, int y, int ghTileset) const
{
	// 空の場合は表示しない
	if (m_tileType == TileType::Empty) return;

	if (m_digAnimationState == DigAnimationState::NotDigging)
	{
		// 掘っているブロックの描画
		POINT pos = TILE_SPRITES[static_cast<int>(m_tileType)];
		// タイルの描画
		DrawRectGraph(x, y
			, TILE_PIXEL_WIDTH * pos.x, TILE_PIXEL_HEIGHT * pos.y
			, TILE_PIXEL_WIDTH, TILE_PIXEL_HEIGHT, ghTileset, TRUE);
	}
	else
	{
		// 掘っているブロックの描画
		POINT pos = DIG_BRICK_SPRITES[static_cast<int>(m_digAnimationState)];
		// タイルの描画
		DrawRectGraph(x, y
			, TILE_PIXEL_WIDTH * pos.x, TILE_PIXEL_HEIGHT * pos.y
			, TILE_PIXEL_WIDTH, TILE_PIXEL_HEIGHT, ghTileset, TRUE);
	}
}

// タイルが移動可能か調べる関数（上左右）
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

// タイルが移動可能か調べる関数（下）
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

// タイルが落下可能か調べる関数（落下）
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


