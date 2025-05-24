//--------------------------------------------------------------------------------------
// File: String.cpp
//
//--------------------------------------------------------------------------------------
#include "String.h"
#include "Tile.h"

// コンストラクタ
String::String(POINT position, const char* string)
	: m_position{ position }
	, m_string{ string }
{
}

// 描画関数
void String::Render(int ghTileset) const
{
	// 数字の表示
	for (size_t i = 0; i < m_string.length(); i++)
	{
		if ((m_string[i] >= 'A') && (m_string[i] <= 'Z'))
		{
			int pos = m_string[i] - 'A';
			DrawRectGraph(m_position.x + i * Tile::TILE_WIDTH, m_position.y
				, (pos % 10) * Tile::TILE_WIDTH, ((pos / 10) + 1) * Tile::TILE_HEIGHT, Tile::TILE_WIDTH, Tile::TILE_HEIGHT
				, ghTileset, TRUE);

		}
	}
}

