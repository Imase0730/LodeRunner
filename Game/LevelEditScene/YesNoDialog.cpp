//--------------------------------------------------------------------------------------
// File: YesNoDialog.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "YesNoDialog.h"

// コンストラクタ
YesNoDialog::YesNoDialog()
	: m_isVisible{ false }
	, m_position{ POINT{ 0, 0 } }
	, m_yesString{ POINT{ 0, 0 }, "YES" }
	, m_noString{ POINT{ 0, 0 }, "NO" }
	, m_yesNo{ YesNo::No }
{
}

// 更新処理
bool YesNoDialog::Update(int keyTrigger)
{
	// 左キーが押された
	if (keyTrigger & PAD_INPUT_LEFT)
	{
		m_yesNo = YesNo::Yes;
	}

	// 右キーが押された
	if (keyTrigger & PAD_INPUT_RIGHT)
	{
		m_yesNo = YesNo::No;
	}

	// 決定キー(Z)が押された
	if (keyTrigger & PAD_INPUT_1)
	{
		// 表示OFF
		m_isVisible = false;
		return true;
	}

	return false;
}

// 描画処理
void YesNoDialog::Render(int ghTileset)
{
	// 表示ON？
	if (!m_isVisible) return;

	// 下地
	DrawBox(m_position.x - 6, m_position.y - 2
		, m_position.x + Tile::TILE_PIXEL_WIDTH * 6 + 5, m_position.y + Tile::TILE_PIXEL_HEIGHT + 5
		, GetColor(0, 0, 0), TRUE);
	DrawBox(m_position.x - 6, m_position.y - 2
		, m_position.x + Tile::TILE_PIXEL_WIDTH * 6 + 5, m_position.y + Tile::TILE_PIXEL_HEIGHT + 5
		, GetColor(255, 255, 255), FALSE);

	// Yes
	SetDrawBright(100, 100, 100);
	if (m_yesNo == YesNo::Yes) SetDrawBright(255, 255, 255);
	m_yesString.SetPosition(POINT{ m_position.x, m_position.y });
	m_yesString.Render(ghTileset);

	// No
	SetDrawBright(100, 100, 100);
	if (m_yesNo == YesNo::No) SetDrawBright(255, 255, 255);
	m_noString.SetPosition(POINT{ m_position.x + Tile::TILE_PIXEL_WIDTH * 4, m_position.y });
	m_noString.Render(ghTileset);

	// 明るさを戻す
	SetDrawBright(255, 255, 255);
}

// ダイアログを起動する関数
void YesNoDialog::StartDialog(POINT position)
{
	// 位置
	m_position = position;

	// 表示ON
	m_isVisible = true;

	// カーソル位置（No）
	m_yesNo = YesNo::No;
}
