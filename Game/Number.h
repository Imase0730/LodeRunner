//--------------------------------------------------------------------------------------
// File: Number.h
//
//--------------------------------------------------------------------------------------
#pragma once

// �����\���N���X
class Number
{
	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �ʒu
	POINT m_position;

	// ����
	int m_digit;

	// ����
	int m_number;

	// �\���̍ő�l
	int m_max;

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	Number(POINT position, int digit);

	// �`�揈��
	void Render(int ghTileset) const;

	// �����̐ݒ�֐�
	void SetNumber(int number) { m_number = number; }

};

