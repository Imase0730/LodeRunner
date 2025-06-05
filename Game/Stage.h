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
private:

	// �@���������K�̏��
	struct DigBrick
	{
		POINT position;	// �ʒu
		int timer;		// �����^�C�}�[
	};

	// �N���X�萔�̐錾 -------------------------------------------------
public:

	// �X�e�[�W�f�[�^�̃p�X
	static constexpr char STAGE_DATA_PATH[] = "Resources/StageData/";

	// �X�e�[�W�T�C�Y
	static constexpr int STAGE_WIDTH = 28;
	static constexpr int STAGE_HEIGHT = 16;

	// �X�e�[�W�T�C�Y
	static constexpr int STAGE_SCREEN_WIDTH = STAGE_WIDTH * Tile::TILE_PIXEL_WIDTH;
	static constexpr int STAGE_SCREEN_HEIGHT = STAGE_HEIGHT * Tile::TILE_PIXEL_HEIGHT;

	// �X�e�[�W��ɒu����G�̍ő吔
	static constexpr int ENEMY_MAX = 5;

	// �@���������K�����L�����郏�[�N�̍ő吔
	static constexpr int DIG_BRICK_MAX = 30;

	// �@���������K�̕�������܂ł̃t���[����
	static constexpr int BRICK_FILL_FRAME = 180;

	// �����K�̕����A�j���[�V�����p
	static constexpr int BRICK_ANIME_TIME_FILL01 = 20;
	static constexpr int BRICK_ANIME_TIME_FILL02 = 10;

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �ǂݍ��񂾃X�e�[�W�f�[�^
	Tile::TileType m_loadData[STAGE_HEIGHT][STAGE_WIDTH];

	// �X�e�[�W�f�[�^
	Tile m_stageData[STAGE_HEIGHT][STAGE_WIDTH];

	// ���[�h
	Mode m_mode;

	// �\�����̃��x��
	int m_level;

	// �X�e�[�W��̓G�̐�
	int m_enemyCount;

	// �v���C���[�̏����ʒu
	POINT m_playerPosition;

	// �G�̏����ʒu
	POINT m_enemyPosition[ENEMY_MAX];

	// �@���������K�̏��L�^�p
	DigBrick m_digBrick[DIG_BRICK_MAX];

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	Stage(Mode mode);

	// �f�X�g���N�^
	~Stage();

	// ������
	void Initialize(Mode mode);

	// �X�V����
	void Update();

	// �`�揈��
	void Render(int ghTileset) const;

	// �w��ꏊ�̃^�C����ݒ肷��֐�
	void SetTileType(int x, int y, Tile::TileType type) { m_stageData[y][x].SetTileType(type); }

	// �w��ꏊ�̃^�C���̌@��A�j���[�V������ݒ肷��֐�
	void SetTileDigAnimationState(int x, int y, Tile::DigAnimationState state) { m_stageData[y][x].SetDigAnimationState(state); }

	// �w��ꏊ�̃^�C�����擾����֐�
	Tile::TileType GetTileType(int x, int y);

	// �w�背�x�����Z�[�u����֐�
	bool SaveLevel(int level);

	// �w�背�x�������[�h����֐�
	bool LoadLevel(int level, Mode mode);

	// ���x�����擾����֐�
	int GetLevel() const { return m_level; }

	// �n�V�S���o������֐�
	void AppearLadder();

	// �v���C���[�̈ʒu���擾����֐�
	POINT GetPlayerPosition() const { return m_playerPosition; }

	// �w��ʒu�̃����K�𕜌�����
	void SetFillBrick(int x, int y);

};

