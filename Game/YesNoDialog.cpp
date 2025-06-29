//--------------------------------------------------------------------------------------
// File: YesNoDialog.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "YesNoDialog.h"

// �R���X�g���N�^
YesNoDialog::YesNoDialog()
	: m_isVisible{ false }
	, m_yesString{ POINT{ 0, 0 }, "YES" }
	, m_noString{ POINT{ 0, 0 }, "NO" }
	, m_yesNo{ YesNo::No }
	, m_title{ "" }
	, m_titleString{ POINT{ 0, 0 }, "" }
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

	// �\���ʒu�𒲐�
	int yesNoWidth = 6;
	int dialogWidth = yesNoWidth;
	int dialogHeight = 1;
	int titleWidth = static_cast<int>(m_title.size());
	if (titleWidth > dialogWidth) dialogWidth = titleWidth;
	if (titleWidth) dialogHeight += 2;

	POINT position = POINT{ (Screen::GAME_WIDTH - Tile::TILE_PIXEL_WIDTH * dialogWidth) / 2, Tile::TILE_PIXEL_HEIGHT * 6 };

	// ���n
	DrawBox(position.x - 6, position.y - 2
		, position.x + Tile::TILE_PIXEL_WIDTH * dialogWidth + 5, position.y + (Tile::TILE_PIXEL_HEIGHT * dialogHeight) + 5
		, GetColor(0, 0, 0), TRUE);
	DrawBox(position.x - 6, position.y - 2
		, position.x + Tile::TILE_PIXEL_WIDTH * dialogWidth + 5, position.y + (Tile::TILE_PIXEL_HEIGHT * dialogHeight) + 5
		, GetColor(255, 255, 255), FALSE);

	// �^�C�g��
	if (titleWidth)
	{
		int offsetX = Tile::TILE_PIXEL_WIDTH * (dialogWidth - titleWidth) / 2;
		m_titleString.SetPosition(POINT{ position.x + offsetX, position.y });
		m_titleString.Render(ghTileset);
	}

	int offsetX = Tile::TILE_PIXEL_WIDTH * (dialogWidth - yesNoWidth) / 2;

	// Yes
	SetDrawBright(100, 100, 100);
	if (m_yesNo == YesNo::Yes) SetDrawBright(255, 255, 255);
	m_yesString.SetPosition(POINT{ position.x + offsetX, position.y + Tile::TILE_PIXEL_HEIGHT * (dialogHeight - 1) });
	m_yesString.Render(ghTileset);

	// No
	SetDrawBright(100, 100, 100);
	if (m_yesNo == YesNo::No) SetDrawBright(255, 255, 255);
	m_noString.SetPosition(POINT{ position.x + offsetX + Tile::TILE_PIXEL_WIDTH * 4, position.y + +Tile::TILE_PIXEL_HEIGHT * (dialogHeight - 1) });
	m_noString.Render(ghTileset);

	// ���邳��߂�
	SetDrawBright(255, 255, 255);
}

// �_�C�A���O���N������֐�
void YesNoDialog::StartDialog(std::string title)
{
	// �^�C�g��
	m_title = title;
	m_titleString.SetString(title.c_str());

	// �\��ON
	m_isVisible = true;

	// �J�[�\���ʒu�iNo�j
	m_yesNo = YesNo::No;
}
