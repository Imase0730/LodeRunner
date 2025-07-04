//--------------------------------------------------------------------------------------
// File: StringRenderer.cpp
//
//--------------------------------------------------------------------------------------
#include "StringRenderer.h"
#include "Tile.h"

// コンストラクタ
StringRenderer::StringRenderer(POINT position, const char* string)
	: m_position{ position }
	, m_string{ string }
{
}

// 描画処理
void StringRenderer::Render(int ghTileset) const
{
	// 数字の表示
	for (int i = 0; i < m_string.length(); i++)
	{
		if ((m_string[i] >= 'A') && (m_string[i] <= '['))
		{
			int pos = m_string[i] - 'A';
			DrawRectGraph(m_position.x + i * Tile::TILE_PIXEL_WIDTH, m_position.y
				, (pos % 10) * Tile::TILE_PIXEL_WIDTH, ((pos / 10) + 1) * Tile::TILE_PIXEL_HEIGHT
				, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT
				, ghTileset, TRUE);

		}
	}
}

