//--------------------------------------------------------------------------------------
// File: Blink.h
//
//--------------------------------------------------------------------------------------
#pragma once

// �_�ŃN���X
class Blink
{
private:

	// �_�ŊԊu
	int m_intervalFrame;

	// �^�C�}�[
	int m_timer;

	// �_��(ON/OFF)
	bool m_isBlinkOn;

public:

	// �R���X�g���N�^
	Blink(int intervalFrame);

	// �X�V����
	void Update();

	// �_�ł̊������擾����֐�
	float GetBlinkRate() const;

	// �_�ł���(ON/OFF)���擾����֐�
	bool IsBlinkOn() const { return m_isBlinkOn; }

	// �_�ŊԊu�̐ݒ�֐�
	void SetInterval(int intervalFrame) { m_intervalFrame = intervalFrame; }

};