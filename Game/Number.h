//--------------------------------------------------------------------------------------
// File: Number.h
//
//--------------------------------------------------------------------------------------
#pragma once

// 数字表示クラス
class Number
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

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Number(POINT position, int digit);

	// 描画処理
	void Render(int ghTileset) const;

	// 数字の設定関数
	void SetNumber(int number) { m_number = number; }

};

