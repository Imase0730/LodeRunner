//--------------------------------------------------------------------------------------
// File: Blink.cpp
//
//--------------------------------------------------------------------------------------
#include "Blink.h"

// コンストラクタ
Blink::Blink(int intervalFrame)
	: m_intervalFrame{ intervalFrame }
	, m_timer{}
{
}

// 更新処理
void Blink::Update()
{
	if (++m_timer > m_intervalFrame)
	{
		m_timer = 0;
	}
}

// 点滅の割合を取得する関数
float Blink::GetBlinkRate() const
{
	// 0.0 → 1.0 → 0.0
	if (m_timer <= m_intervalFrame / 2)
	{
		return m_timer / (m_intervalFrame / 2.0f);
	}
	return (m_intervalFrame - m_timer) / (m_intervalFrame / 2.0f);
}
