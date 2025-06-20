//--------------------------------------------------------------------------------------
// File: YesNoDialog.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "YesNoDialog.h"

// �R���X�g���N�^
YesNoDialog::YesNoDialog()
	: m_isVisible{ false }
	, m_position{ POINT{ 0, 0 } }
	, m_yesString{ POINT{ 0, 0 }, "YES" }
	, m_noString{ POINT{ 0, 0 }, "NO" }
	, m_yesNo{ YesNo::No }
{
}

// �X�V����
bool YesNoDialog::Update(int keyTrigger)
{
	// ���L�[�������ꂽ
	if (keyTrigger & PAD_INPUT_LEFT)
	{
		m_yesNo = YesNo::Yes;
	}

	// �E�L�[�������ꂽ
	if (keyTrigger & PAD_INPUT_RIGHT)
	{
		m_yesNo = YesNo::No;
	}

	// ����L�[(Z)�������ꂽ
	if (keyTrigger & PAD_INPUT_1)
	{
		// �\��OFF
		m_isVisible = false;
		return true;
	}

	return false;
}

// �`�揈��
void YesNoDialog::Render(int ghTileset)
{
	// �\��ON�H
	if (!m_isVisible) return;

	// ���n
	DrawBox(m_position.x - 6, m_position.y - 2
		, m_position.x + Tile::TILE_PIXEL_WIDTH * 6 + 5, m_position.y + Tile::TILE_PIXEL_HEIGHT + 5
		, GetColor(0, 0, 0), TRUE);
	DrawBox(m_position.x - 6, m_position.y - 2
		, m_position.x + Tile::TILE_PIXEL_WIDTH * 6 + 5, m_position.y + Tile::TILE_PIXEL_HEIGHT + 5
		, GetColor(255, 255, 255), FALSE);

	// Yes
	SetDrawBright(100, 100, 100);
	if (m_yesNo == YesNo::Yes) SetDrawBright(255, 255, 255);
	m_yesString.SetPosition(POINT{ m_position.x, m_position.y });
	m_yesString.Render(ghTileset);

	// No
	SetDrawBright(100, 100, 100);
	if (m_yesNo == YesNo::No) SetDrawBright(255, 255, 255);
	m_noString.SetPosition(POINT{ m_position.x + Tile::TILE_PIXEL_WIDTH * 4, m_position.y });
	m_noString.Render(ghTileset);

	// ���邳��߂�
	SetDrawBright(255, 255, 255);
}

// �_�C�A���O���N������֐�
void YesNoDialog::StartDialog(POINT position)
{
	// �ʒu
	m_position = position;

	// �\��ON
	m_isVisible = true;

	// �J�[�\���ʒu�iNo�j
	m_yesNo = YesNo::No;
}
