//--------------------------------------------------------------------------------------
// File: YesNoDialog.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/StringRenderer.h"
#include "Game/Tile.h"
#include "Game/Screen.h"

class YesNoDialog
{
public:

	enum class YesNo
	{
		Yes,
		No,
	};

private:

	// �\���t���O
	bool m_isVisible;

	// �\���ʒu
	POINT m_position;

	// YES�̕���
	StringRenderer m_yesString;

	// NO�̕���
	StringRenderer m_noString;

	// �ǂ����I���H
	YesNo m_yesNo;

public:

	// �R���X�g���N�^
	YesNoDialog();

	// �X�V����
	bool Update(int keyTrigger);

	// �`�揈��
	void Render(int ghTileset);

	// �\�����Ă��邩���ׂ�֐�
	bool IsVisivle() const { return m_isVisible; }

	// �_�C�A���O���N������֐�
	void StartDialog(POINT position = POINT{ (Screen::GAME_WIDTH - Tile::TILE_PIXEL_WIDTH * 6) / 2, Tile::TILE_PIXEL_HEIGHT * 7 });

	// �ǂ��炪�I�����ꂽ���擾����֐�
	YesNo GetYesNo() const { return m_yesNo; }

};
