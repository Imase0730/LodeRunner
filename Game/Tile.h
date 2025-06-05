//--------------------------------------------------------------------------------------
// File: Tile.h
//
//--------------------------------------------------------------------------------------
#pragma once

class Tile
{
	// �񋓌^�̐錾 -------------------------------------------------
public:

	// �^�C���^�C�v
	enum class TileType
	{
		Empty,				// �Ȃ�
		Blick,				// �����K
		Stone,				// �󂹂Ȃ��u���b�N
		Ladder,				// �n�V�S
		Rope,				// ���[�v
		Trap,				// �g���b�v
		InvisibleLadder,	// �B��n�V�S
		Gold,				// ����
		Enemy,				// �G
		Player,				// �v���C���[

		TileTypeMax,
	};

	// �@��A�j���[�V�����C���f�b�N�X
	enum class DigAnimationState
	{
		NotDigging = -1,
		Dig01, Dig02, Dig03, Dig04, Dig05, Dig06,
		Dig07, Dig08, Dig09, Dig10, Dig11, Dig12,
		Fill01, Fill02,
	};

	// �\���̂̐錾 -------------------------------------------------
public:


	// �N���X�萔�̐錾 -------------------------------------------------
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

	// �^�C���̊G�̈ʒu
	static constexpr POINT TILE_SPRITES[] =
	{
		{ 0, 4 },	// �Ȃ�
		{ 1, 4 },	// �����K
		{ 2, 4 },	// �󂹂Ȃ��u���b�N
		{ 3, 4 },	// �n�V�S
		{ 4, 4 },	// ���[�v
		{ 5, 4 },	// �g���b�v
		{ 6, 4 },	// �B��n�V�S
		{ 7, 4 },	// ����
		{ 8, 4 },	// �G
		{ 9, 4 },	// �v���C���[
	};

	// �@���Ă��郌���K�̊G�̈ʒu
	static constexpr POINT DIG_BRICK_SPRITES[] =
	{
		{ 0, 10 }, { 0, 10 },
		{ 1, 10 }, { 1, 10 },
		{ 2, 10 }, { 2, 10 },
		{ 3, 10 }, { 3, 10 },
		{ 4, 10 }, { 4, 10 },
		{ 5, 10 }, { 5, 10 },

		{ 6, 10 }, { 7, 10 },
	};

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �^�C���^�C�v
	TileType m_tileType;

	// �@��A�j���[�V�����X�e�[�g
	DigAnimationState m_digAnimationState;

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	Tile();

	// �`�揈��
	void Render(int x, int y, int ghTileset) const;

	// �^�C���^�C�v�̐ݒ�֐�
	void SetTileType(TileType type) { m_tileType = type; }

	// �^�C���^�C�v�̎擾�֐�
	TileType GetTileType() const { return m_tileType; }

	// �@��A�j���[�V�����X�e�[�g�̎擾�֐�
	DigAnimationState GetDigAnimationState() const { return m_digAnimationState; }

	// �@��A�j���[�V�����X�e�[�g�̐ݒ�֐�
	void SetDigAnimationState(DigAnimationState state) { m_digAnimationState = state; }

	// �ړ��\�ȃ^�C�������ׂ�֐��i�㍶�E�j
	static bool IsMovableTileULR(TileType tileType);

	// �ړ��\�ȃ^�C�������ׂ�֐��i���j
	static bool IsMovableTileDown(TileType tileType);

	// �ړ��\�ȃ^�C�������ׂ�֐��i�����j
	static bool IsMovableTileFall(TileType tileType);

};

