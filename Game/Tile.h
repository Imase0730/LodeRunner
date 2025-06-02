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

	// �\���̂̐錾 -------------------------------------------------
public:


	// �N���X�萔�̐錾 -------------------------------------------------
public:

	// �^�C���̃s�N�Z���T�C�Y
	static constexpr int TILE_PIXEL_WIDTH = 10;
	static constexpr int TILE_PIXEL_HEIGHT = 11;

	// �^�C���̃T�C�Y
	static constexpr int TILE_WIDTH = 5;
	static constexpr int TILE_HEIGHT = 5;

	// �^�C���̒��S�܂ł̋���
	static constexpr int TILE_CENTER_X = 2;
	static constexpr int TILE_CENTER_Y = 2;

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �^�C���^�C�v
	TileType m_tileType;

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	Tile();

	// �f�X�g���N�^
	~Tile();

	// ������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Render(int x, int y, int ghTileset) const;

	// �^�C���^�C�v�̐ݒ�֐�
	void SetTileType(TileType type) { m_tileType = type; }

	// �^�C���^�C�v�̎擾�֐�
	TileType GetTileType() const { return m_tileType; }

	// �^�C�����ړ��\�����ׂ�֐��i�㍶�E�j
	static bool IsMovableTileULR(TileType tileType);

	// �^�C�����ړ��\�����ׂ�֐��i���j
	static bool IsMovableTileDown(TileType tileType);

	// �^�C���������\�����ׂ�֐��i�����j
	static bool IsMovableTileFall(TileType tileType);

};

