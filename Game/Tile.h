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
		None,
		Blick,			// �����K
		Block,			// �󂹂Ȃ��u���b�N
		Ladder,			// �n�V�S
		Bar,			// �o�[
		Trap,			// �g���b�v
		HiddenLadder,	// �B��n�V�S
		Gold,			// ����
		Enemy,			// �G
		Player,			// �v���C���[

		TileTypeMax,
	};

	// �\���̂̐錾 -------------------------------------------------
public:


	// �N���X�萔�̐錾 -------------------------------------------------
public:

	// �^�C���̃T�C�Y
	static constexpr int TILE_WIDTH = 10;
	static constexpr int TILE_HEIGHT = 11;

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

};

