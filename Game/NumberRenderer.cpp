//--------------------------------------------------------------------------------------
// File: NumberRenderer.cpp
//
//--------------------------------------------------------------------------------------
#include "NumberRenderer.h"
#include "Tile.h"

// コンストラクタ
NumberRenderer::NumberRenderer(POINT position, int digit, bool isZeroPadding)
	: m_position{ position }
	, m_digit{ digit }
	, m_isZeroPadding{ isZeroPadding }
	, m_number{ 0 }
	, m_max{ 0 }
{
	// 表示の最大値を求める（3桁なら999）
	m_max = 1;
	for (int i = 0; i < m_digit; i++)
	{
		m_max *= 10;
	}
	m_max -= 1;
}

// 描画処理
void NumberRenderer::Render(int ghTileset) const
{
	int number = m_number;

	// 桁数を超えた場合は桁数の最大値の表示にする
	if (number > m_max) number = m_max;

	// 数字の表示
	for (int i = 0; i < m_digit; i++)
	{
		DrawRectGraph( m_position.x + ((m_digit - 1) - i) * Tile::TILE_PIXEL_WIDTH, m_position.y
					 , (number % 10) * Tile::TILE_PIXEL_WIDTH, 0
					 , Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT
					 , ghTileset, TRUE);
		number /= 10;

		if ((number == 0) && (m_isZeroPadding == false)) break;
	}
}
