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

	// タイトルの文字列
	std::string m_title;

	// タイトルの文字
	StringRenderer m_titleString;

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

	// ダイアログを閉じる関数
	void CloseDialog() { m_isVisible = false; }

	// ダイアログを起動する関数
	void StartDialog(std::string title = "");

	// どちらが選択されたか取得する関数
	YesNo GetYesNo() const { return m_yesNo; }

};
