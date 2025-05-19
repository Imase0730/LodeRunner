//--------------------------------------------------------------------------------------
// File: Tile.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Tile.h"
#include "Stage.h"
#include "Game/Game.h"

Tile::Tile()
	: m_tileType{ TileType::None }
{
}

Tile::~Tile()
{
}

void Tile::Initialize()
{
}

void Tile::Update()
{
}

void Tile::Render(int x, int y, int ghTileset) const
{
	switch (m_tileType)
	{
	case TileType::Blick:			// レンガ
		DrawRectGraph(x, y, TILE_WIDTH * 0, TILE_HEIGHT * 1, TILE_WIDTH, TILE_HEIGHT, ghTileset, TRUE);
		break;
	case TileType::Ladder:			// ハシゴ
		DrawRectGraph(x, y, TILE_WIDTH * 1, TILE_HEIGHT * 1, TILE_WIDTH, TILE_HEIGHT, ghTileset, TRUE);
		break;
	case TileType::Bar:				// バー
		DrawRectGraph(x, y, TILE_WIDTH * 2, TILE_HEIGHT * 1, TILE_WIDTH, TILE_HEIGHT, ghTileset, TRUE);
		break;
	case TileType::Block:			// 壊せないブロック
		DrawRectGraph(x, y, TILE_WIDTH * 3, TILE_HEIGHT * 1, TILE_WIDTH, TILE_HEIGHT, ghTileset, TRUE);
		break;
	case TileType::Gold:			// 金塊
		DrawRectGraph(x, y, TILE_WIDTH * 4, TILE_HEIGHT * 1, TILE_WIDTH, TILE_HEIGHT, ghTileset, TRUE);
		break;
	case TileType::HiddenLadder:	// 隠れハシゴ
		DrawRectGraph(x, y, TILE_WIDTH * 5, TILE_HEIGHT * 1, TILE_WIDTH, TILE_HEIGHT, ghTileset, TRUE);
		break;
	case TileType::Trap:			// トラップ
		DrawRectGraph(x, y, TILE_WIDTH * 6, TILE_HEIGHT * 1, TILE_WIDTH, TILE_HEIGHT, ghTileset, TRUE);
		break;
	case TileType::Enemy:			// 敵
		DrawRectGraph(x, y, TILE_WIDTH * 7, TILE_HEIGHT * 1, TILE_WIDTH, TILE_HEIGHT, ghTileset, TRUE);
		break;
	case TileType::Player:			// プレイヤー
		DrawRectGraph(x, y, TILE_WIDTH * 8, TILE_HEIGHT * 1, TILE_WIDTH, TILE_HEIGHT, ghTileset, TRUE);
		break;
	default:
		break;
	}
}

