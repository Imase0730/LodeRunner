//--------------------------------------------------------------------------------------
// File: Tile.h
//
//--------------------------------------------------------------------------------------
#pragma once

// �^�C���N���X
class Tile
{
	// �񋓌^�̐錾 -------------------------------------------------
public:

	// �^�C���̃^�C�v
	enum class Type
	{
		Empty,				// �Ȃ�
		Blick,				// �����K
		Stone,				// ��
		Ladder,				// �n�V�S
		Rope,				// ���[�v
		Trap,				// �g���b�v
		InvisibleLadder,	// �B��n�V�S
		Gold,				// ����
		Guard,				// �K�[�h
		Player,				// �v���C���[
	};

	// �N���X�萔�̐錾 ---------------------------------------------
public:

	// �^�C���̃s�N�Z���T�C�Y�i�\���p�j
	static constexpr int TILE_PIXEL_WIDTH = 10;
	static constexpr int TILE_PIXEL_HEIGHT = 11;

	// �^�C���̃T�C�Y�i�v���O������ł̍��W�v�Z�p�j
	static constexpr int TILE_WIDTH = 5;
	static constexpr int TILE_HEIGHT = 5;

	// �^�C���̒��S�܂ł̋����i�v���O������ł̍��W�v�Z�p�j
	static constexpr int TILE_CENTER_X = 2;
	static constexpr int TILE_CENTER_Y = 2;

};
