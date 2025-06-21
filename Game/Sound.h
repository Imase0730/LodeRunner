#pragma once

class Sound
{
public:

	// ���ʉ�
	enum class SoundID
	{
		PickupGold,		// ������E����
		Dig,			// �����@�鉹
		InHole,			// ���ɗ�������
		Resurrection,	// ������
		Dead,			// ���S��
		Clear,			// �N���A��
		Fall,			// ������
		Land,			// ���n��

		SoundIDMax
	};

private:

	// ���ʉ��̃n���h��
	int m_shHandle[static_cast<int>(SoundID::SoundIDMax)];

public:

	Sound();

	~Sound();

	// ���ʉ��̃n���h�����擾����֐�
	int GetSoundHandle(SoundID soundID) const { return m_shHandle[static_cast<int>(soundID)]; }

};