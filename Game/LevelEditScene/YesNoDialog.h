#pragma once

#include "Game/StringRenderer.h"

class YesNoDialog
{
public:

private:

	// 表示フラグ
	bool m_isVisible;

	// 表示位置
	POINT m_position;

	// YESの文字
	StringRenderer m_yesString;

	// NOの文字
	StringRenderer m_noString;

public:

	// コンストラクタ
	YesNoDialog();

	// 描画処理
	void Render(int ghTileset);

	// 表示しているか調べる関数
	bool IsVisivle() const { return m_isVisible; }

};
