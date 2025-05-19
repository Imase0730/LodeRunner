//--------------------------------------------------------------------------------------
// File: Stage.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Tile.h"

// �N���X�̑O���錾
class GameplayScene;

class Stage
{
	// �񋓌^�̐錾 -------------------------------------------------
public:


	// �\���̂̐錾 -------------------------------------------------
public:


	// �N���X�萔�̐錾 -------------------------------------------------
public:

	// �X�e�[�W�T�C�Y
	static constexpr int STAGE_WIDTH = 28;
	static constexpr int STAGE_HEIGHT = 16;

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �Q�[���V�[���ւ̃|�C���^
	GameplayScene* m_pGameplayScene;

	// �O���t�B�b�N�n���h��
	int m_ghGame_bottom;
	int m_ghEdit_bottom;

	// �}�b�v�f�[�^
	Tile m_tileMap[STAGE_HEIGHT][STAGE_WIDTH];


	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	Stage(GameplayScene* scene);

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
};

