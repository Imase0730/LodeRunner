#include "pch.h"
#include "YesNoDialog.h"
#include "Game/Tile.h"
#include "Game/Screen.h"

// コンストラクタ
YesNoDialog::YesNoDialog()
	: m_isVisible{ false }
	, m_position{ POINT{ (Screen::WIDTH - Tile::TILE_PIXEL_WIDTH * 6) / 2, 0 } }
	, m_yesString{ POINT{ 0, 0 }, "YES" }
	, m_noString{ POINT{ 0, 0 }, "NO" }
{
}

// 描画処理
void YesNoDialog::Render(int ghTileset)
{
	m_yesString.SetPosition(POINT{ m_position.x, m_position.y });
	m_yesString.Render(ghTileset);
	m_noString.SetPosition(POINT{ m_position.x + Tile::TILE_PIXEL_WIDTH * 4, m_position.y });
	m_noString.Render(ghTileset);
}
