//--------------------------------------------------------------------------------------
// File: Stage.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Tile.h"

class Stage
{
	// �񋓌^�̐錾 -------------------------------------------------
public:

	// ���[�h
	enum class Mode
	{
		GamePlay,
		StageEdit,
	};

	// �\���̂̐錾 -------------------------------------------------
public:


	// �N���X�萔�̐錾 -------------------------------------------------
public:

	// �X�e�[�W�f�[�^�̃p�X
	static constexpr char STAGE_DATA_PATH[] = "Resources/StageData/";

	// �X�e�[�W�T�C�Y
	static constexpr int STAGE_WIDTH = 28;
	static constexpr int STAGE_HEIGHT = 16;

	// �X�e�[�W�T�C�Y
	static constexpr int STAGE_SCREEN_WIDTH = STAGE_WIDTH * Tile::TILE_WIDTH;
	static constexpr int STAGE_SCREEN_HEIGHT = STAGE_HEIGHT * Tile::TILE_HEIGHT;

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �}�b�v�f�[�^
	Tile m_tileMap[STAGE_HEIGHT][STAGE_WIDTH];

	// ���[�h
	Mode m_mode;

	// �\�����̃��x��
	int m_level;

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	Stage(Mode mode);

	// �f�X�g���N�^
	~Stage();

	// ������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Render(int ghTileset) const;

	// �w��ꏊ�̃^�C����ݒ肷��֐�
	void SetTileType(int x, int y, Tile::TileType type) { m_tileMap[y][x].SetTileType(type); }

	// �w��ꏊ�̃^�C�����擾����֐�
	Tile::TileType GetTileType(int x, int y) { return m_tileMap[y][x].GetTileType(); }

	// �w�背�x�����Z�[�u����֐�
	bool SaveLevel(int level);

	// �w�背�x�������[�h����֐�
	bool LoadLevel(int level);

	// ���x�����擾����֐�
	int GetLevel() const { return m_level; }

};

