//--------------------------------------------------------------------------------------
// File: Blink.h
//
//--------------------------------------------------------------------------------------
#pragma once

// 点滅クラス
class Blink
{
private:

	// 点滅間隔
	int m_intervalFrame;

	// タイマー
	int m_timer;

	// 点滅(ON/OFF)
	bool m_isBlinkOn;

public:

	// コンストラクタ
	Blink(int intervalFrame);

	// 更新処理
	void Update();

	// 点滅の割合を取得する関数
	float GetBlinkRate() const;

	// 点滅する(ON/OFF)を取得する関数
	bool IsBlinkOn() const { return m_isBlinkOn; }

	// 点滅間隔の設定関数
	void SetInterval(int intervalFrame) { m_intervalFrame = intervalFrame; }

};