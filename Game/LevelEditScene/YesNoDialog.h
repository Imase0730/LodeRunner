//--------------------------------------------------------------------------------------
// File: YesNoDialog.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/StringRenderer.h"
#include "Game/Tile.h"
#include "Game/Screen.h"

class YesNoDialog
{
public:

	enum class YesNo
	{
		Yes,
		No,
	};

private:

	// 表示フラグ
	bool m_isVisible;

	// 表示位置
	POINT m_position;

	// YESの文字
	StringRenderer m_yesString;

	// NOの文字
	StringRenderer m_noString;

	// どちらを選択？
	YesNo m_yesNo;

public:

	// コンストラクタ
	YesNoDialog();

	// 更新処理
	bool Update(int keyTrigger);

	// 描画処理
	void Render(int ghTileset);

	// 表示しているか調べる関数
	bool IsVisivle() const { return m_isVisible; }

	// ダイアログを起動する関数
	void StartDialog(POINT position = POINT{ (Screen::GAME_WIDTH - Tile::TILE_PIXEL_WIDTH * 6) / 2, Tile::TILE_PIXEL_HEIGHT * 7 });

	// どちらが選択されたか取得する関数
	YesNo GetYesNo() const { return m_yesNo; }

};
