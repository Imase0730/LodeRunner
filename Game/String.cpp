//--------------------------------------------------------------------------------------
// File: String.cpp
//
//--------------------------------------------------------------------------------------
#include "String.h"
#include "Level.h"

// �R���X�g���N�^
String::String(POINT position, const char* string)
	: m_position{ position }
	, m_string{ string }
{
}

// �`�揈��
void String::Render(int ghTileset) const
{
	// �����̕\��
	for (int i = 0; i < m_string.length(); i++)
	{
		if ((m_string[i] >= 'A') && (m_string[i] <= 'Z'))
		{
			int pos = m_string[i] - 'A';
			DrawRectGraph(m_position.x + i * Level::TILE_PIXEL_WIDTH, m_position.y
				, (pos % 10) * Level::TILE_PIXEL_WIDTH, ((pos / 10) + 1) * Level::TILE_PIXEL_HEIGHT
				, Level::TILE_PIXEL_WIDTH, Level::TILE_PIXEL_HEIGHT
				, ghTileset, TRUE);

		}
	}
}

