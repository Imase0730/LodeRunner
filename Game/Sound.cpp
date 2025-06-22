//--------------------------------------------------------------------------------------
// File: Sound.cpp
//
//--------------------------------------------------------------------------------------
#include "Sound.h"

// コンストラクタ
Sound::Sound()
{
	// 効果音のロード
	m_shHandle[static_cast<int>(SoundID::PickupGold)] = LoadSoundMem(L"Resources/Sounds/pickupGold.wav");
	m_shHandle[static_cast<int>(SoundID::Dig)] = LoadSoundMem(L"Resources/Sounds/dig.wav");
	m_shHandle[static_cast<int>(SoundID::InHole)] = LoadSoundMem(L"Resources/Sounds/inHole.wav");
	m_shHandle[static_cast<int>(SoundID::Resurrection)] = LoadSoundMem(L"Resources/Sounds/resurrection.wav");
	m_shHandle[static_cast<int>(SoundID::Dead)] = LoadSoundMem(L"Resources/Sounds/dead.wav");
	m_shHandle[static_cast<int>(SoundID::Clear)] = LoadSoundMem(L"Resources/Sounds/clear.wav");
	m_shHandle[static_cast<int>(SoundID::Fall)] = LoadSoundMem(L"Resources/Sounds/fall.wav");
	m_shHandle[static_cast<int>(SoundID::Land)] = LoadSoundMem(L"Resources/Sounds/land.wav");
}

// デストラクタ
Sound::~Sound()
{
	// サウンドハンドルの削除
	for (int i = 0; i < static_cast<int>(SoundID::SoundIDMax); i++)
	{
		DeleteSoundMem(m_shHandle[i]);
	}
}

// 効果音を鳴らす関数
void Sound::PlaySound(SoundID soundID) const
{
	PlaySoundMem(GetSoundHandle(soundID), DX_PLAYTYPE_BACK);
}

// 効果音を止める関数
void Sound::StopSound(SoundID soundID) const
{
	StopSoundMem(GetSoundHandle(soundID));
}

bool Sound::CheckSound(SoundID soundID) const
{
	return CheckSoundMem(GetSoundHandle(soundID));
}
