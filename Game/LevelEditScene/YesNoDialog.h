#pragma once

#include "Game/StringRenderer.h"

class YesNoDialog
{
public:

private:

	// �\���t���O
	bool m_isVisible;

	// �\���ʒu
	POINT m_position;

	// YES�̕���
	StringRenderer m_yesString;

	// NO�̕���
	StringRenderer m_noString;

public:

	// �R���X�g���N�^
	YesNoDialog();

	// �`�揈��
	void Render(int ghTileset);

	// �\�����Ă��邩���ׂ�֐�
	bool IsVisivle() const { return m_isVisible; }

};
