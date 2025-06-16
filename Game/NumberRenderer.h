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

	// �O�l�߃t���O
	bool m_isZeroPadding;

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	NumberRenderer(POINT position, int digit, bool isZeroPadding = true);

	// �`�揈��
	void Render(int ghTileset) const;

	// �����̐ݒ�֐�
	void SetNumber(int number) { m_number = number; }

	// �\���ʒu��ݒ肷��֐�
	void SetPosition(POINT position) { m_position = position; }

};

