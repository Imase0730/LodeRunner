//--------------------------------------------------------------------------------------
// File: NumberRenderer.cpp
//
//--------------------------------------------------------------------------------------
#include "NumberRenderer.h"
#include "Tile.h"

// �R���X�g���N�^
NumberRenderer::NumberRenderer(POINT position, int digit, bool isZeroPadding)
	: m_position{ position }
	, m_digit{ digit }
	, m_isZeroPadding{ isZeroPadding }
	, m_number{ 0 }
	, m_max{ 0 }
{
	// �\���̍ő�l�����߂�i3���Ȃ�999�j
	m_max = 1;
	for (int i = 0; i < m_digit; i++)
	{
		m_max *= 10;
	}
	m_max -= 1;
}

// �`�揈��
void NumberRenderer::Render(int ghTileset) const
{
	int number = m_number;

	// �����𒴂����ꍇ�͌����̍ő�l�̕\���ɂ���
	if (number > m_max) number = m_max;

	// �����̕\��
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
