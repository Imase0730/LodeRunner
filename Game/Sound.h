//--------------------------------------------------------------------------------------
// File: Sound.h
//
//--------------------------------------------------------------------------------------
#pragma once

// 効果音
class Sound
{
public:

	// 効果音のID
	enum class SoundID
	{
		PickupGold,		// 金塊を拾う音
		Dig,			// 穴を掘る音
		InHole,			// 穴に落ちた音
		Resurrection,	// 復活音
		Dead,			// 死亡音
		Clear,			// クリア音
		Fall,			// 落下音
		Land,			// 着地音

		SoundIDMax
	};

private:

	// 効果音のハンドル
	int m_shHandle[static_cast<int>(SoundID::SoundIDMax)];

public:

	// コンストラクタ
	Sound();

	// デストラクタ
	~Sound();

	// 効果音のハンドルを取得する関数
	int GetSoundHandle(SoundID soundID) const { return m_shHandle[static_cast<int>(soundID)]; }

	// 効果音を鳴らす関数
	void PlaySound(SoundID soundID) const;

	// 効果音を止める関数
	void StopSound(SoundID soundID) const;

	// 効果音が再生中か調べる関数
	bool CheckSound(SoundID soundID) const;
};