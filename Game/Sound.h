//--------------------------------------------------------------------------------------
// File: Sound.h
//
//--------------------------------------------------------------------------------------
#pragma once

// ���ʉ�
class Sound
{
public:

	// ���ʉ���ID
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

	// �R���X�g���N�^
	Sound();

	// �f�X�g���N�^
	~Sound();

	// ���ʉ��̃n���h�����擾����֐�
	int GetSoundHandle(SoundID soundID) const { return m_shHandle[static_cast<int>(soundID)]; }

	// ���ʉ���炷�֐�
	void PlaySound(SoundID soundID) const;

	// ���ʉ����~�߂�֐�
	void StopSound(SoundID soundID) const;

	// ���ʉ����Đ��������ׂ�֐�
	bool CheckSound(SoundID soundID) const;
};