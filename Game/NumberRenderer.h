//--------------------------------------------------------------------------------------
// File: NumberRenderer.h
//
//--------------------------------------------------------------------------------------
#pragma once

// 数字表示クラス
class NumberRenderer
{
	// データメンバの宣言 -----------------------------------------------
private:

	// 位置
	POINT m_position;

	// 桁数
	int m_digit;

	// 数字
	int m_number;

	// 表示の最大値
	int m_max;

	// ０詰めフラグ
	bool m_isZeroPadding;

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	NumberRenderer(POINT position, int digit, bool isZeroPadding = true);

	// 描画処理
	void Render(int ghTileset) const;

	// 数字の設定関数
	void SetNumber(int number) { m_number = number; }

	// 表示位置を設定する関数
	void SetPosition(POINT position) { m_position = position; }

};

