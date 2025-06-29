//--------------------------------------------------------------------------------------
// File: YesNoDialog.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "YesNoDialog.h"

// コンストラクタ
YesNoDialog::YesNoDialog()
	: m_isVisible{ false }
	, m_yesString{ POINT{ 0, 0 }, "YES" }
	, m_noString{ POINT{ 0, 0 }, "NO" }
	, m_yesNo{ YesNo::No }
	, m_title{ "" }
	, m_titleString{ POINT{ 0, 0 }, "" }
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

	// 表示位置を調整
	int yesNoWidth = 6;
	int dialogWidth = yesNoWidth;
	int dialogHeight = 1;
	int titleWidth = static_cast<int>(m_title.size());
	if (titleWidth > dialogWidth) dialogWidth = titleWidth;
	if (titleWidth) dialogHeight += 2;

	POINT position = POINT{ (Screen::GAME_WIDTH - Tile::TILE_PIXEL_WIDTH * dialogWidth) / 2, Tile::TILE_PIXEL_HEIGHT * 6 };

	// 下地
	DrawBox(position.x - 6, position.y - 2
		, position.x + Tile::TILE_PIXEL_WIDTH * dialogWidth + 5, position.y + (Tile::TILE_PIXEL_HEIGHT * dialogHeight) + 5
		, GetColor(0, 0, 0), TRUE);
	DrawBox(position.x - 6, position.y - 2
		, position.x + Tile::TILE_PIXEL_WIDTH * dialogWidth + 5, position.y + (Tile::TILE_PIXEL_HEIGHT * dialogHeight) + 5
		, GetColor(255, 255, 255), FALSE);

	// タイトル
	if (titleWidth)
	{
		int offsetX = Tile::TILE_PIXEL_WIDTH * (dialogWidth - titleWidth) / 2;
		m_titleString.SetPosition(POINT{ position.x + offsetX, position.y });
		m_titleString.Render(ghTileset);
	}

	int offsetX = Tile::TILE_PIXEL_WIDTH * (dialogWidth - yesNoWidth) / 2;

	// Yes
	SetDrawBright(100, 100, 100);
	if (m_yesNo == YesNo::Yes) SetDrawBright(255, 255, 255);
	m_yesString.SetPosition(POINT{ position.x + offsetX, position.y + Tile::TILE_PIXEL_HEIGHT * (dialogHeight - 1) });
	m_yesString.Render(ghTileset);

	// No
	SetDrawBright(100, 100, 100);
	if (m_yesNo == YesNo::No) SetDrawBright(255, 255, 255);
	m_noString.SetPosition(POINT{ position.x + offsetX + Tile::TILE_PIXEL_WIDTH * 4, position.y + +Tile::TILE_PIXEL_HEIGHT * (dialogHeight - 1) });
	m_noString.Render(ghTileset);

	// 明るさを戻す
	SetDrawBright(255, 255, 255);
}

// ダイアログを起動する関数
void YesNoDialog::StartDialog(std::string title)
{
	// タイトル
	m_title = title;
	m_titleString.SetString(title.c_str());

	// 表示ON
	m_isVisible = true;

	// カーソル位置（No）
	m_yesNo = YesNo::No;
}
