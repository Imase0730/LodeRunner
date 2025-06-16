//--------------------------------------------------------------------------------------
// File: StringRenderer.h
//
//--------------------------------------------------------------------------------------
#pragma once

// 文字列表示クラス
class StringRenderer
{
	// データメンバの宣言 -----------------------------------------------
private:

	// 位置
	POINT m_position;

	// 文字列
	std::string m_string;

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	StringRenderer(POINT position, const char* string);

	// 描画処理
	void Render(int ghTileset) const;

	// 文字を設定する関数
	void SetString(const char* string) { m_string = string; }

	// 表示位置を設定する関数
	void SetPosition(POINT position) { m_position = position; }

};

