#pragma once

class Sound
{
public:

	// 効果音
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

	Sound();

	~Sound();

	// 効果音のハンドルを取得する関数
	int GetSoundHandle(SoundID soundID) const { return m_shHandle[static_cast<int>(soundID)]; }

};