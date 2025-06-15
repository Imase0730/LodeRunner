//--------------------------------------------------------------------------------------
// File: NumberRenderer.h
//
//--------------------------------------------------------------------------------------
#pragma once

// �����\���N���X
class NumberRenderer
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
	NumberRenderer(POINT position, int digit);

	// �`�揈��
	void Render(int ghTileset) const;

	// �����̐ݒ�֐�
	void SetNumber(int number) { m_number = number; }

};

