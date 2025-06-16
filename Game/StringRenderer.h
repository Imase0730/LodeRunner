//--------------------------------------------------------------------------------------
// File: StringRenderer.h
//
//--------------------------------------------------------------------------------------
#pragma once

// ������\���N���X
class StringRenderer
{
	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �ʒu
	POINT m_position;

	// ������
	std::string m_string;

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	StringRenderer(POINT position, const char* string);

	// �`�揈��
	void Render(int ghTileset) const;

	// ������ݒ肷��֐�
	void SetString(const char* string) { m_string = string; }

	// �\���ʒu��ݒ肷��֐�
	void SetPosition(POINT position) { m_position = position; }

};

