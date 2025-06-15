//--------------------------------------------------------------------------------------
// File: Gurad.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Level.h"

class GamePlayScene;

// �K�[�h�N���X
class Gurad
{
public:

	// �ړ�����
	enum class MoveDirection
	{
		None,
		UP,
		Down,
		Left,
		Right,
	};

	// ����
	enum class Direction
	{
		Right,	// �E
		Left,	// ��
	};

	// �A�j���[�V�����C���f�b�N�X
	enum class GuradAnimationState
	{
		AnimNone,						// �\�����Ȃ�
		Run01_L, Run02_L, Run03_L,		// ����i�������j
		Rope01_L, Rope02_L, Rope03_L,	// ���[�v�ňړ��i�������j
		Fall_L,							// �������i�������j

		Run01_R, Run02_R, Run03_R,		// ����i�E�����j
		Rope01_R, Rope02_R, Rope03_R,	// ���[�v�ňړ��i�E�����j
		Fall_R,							// �������i�E�����j

		Climb01, Climb02,				// �n�V�S��o����
		Egg01, Egg02,					// ������
	};

	// �N���X�萔�̐錾 -------------------------------------------------
private:

	// �K�[�h�̊G�̈ʒu
	static constexpr POINT GUARD_SPRITES[] =
	{
		// �\�����Ȃ�
		{ 0, 4 },

		// ����i�������j
		{ 0, 7 },
		{ 1, 7 },
		{ 2, 7 },

		// ���[�v�ňړ��i�������j
		{ 0, 8 },
		{ 1, 8 },
		{ 2, 8 },

		// �������i�������j
		{ 8, 7 },

		// ����i�E�����j
		{ 3, 7 },
		{ 4, 7 },
		{ 5, 7 },

		// ���[�v�ňړ��i�E�����j
		{ 3, 8 },
		{ 4, 8 },
		{ 5, 8 },

		// �������i�E�����j
		{ 9, 7 },

		// �n�V�S������Ă���
		{ 6, 7 },
		{ 7, 7 },

		// ������
		{ 8, 10 },
		{ 9, 10 },
	};

	// ����ێ��^�C�}�[�̏����l
	static constexpr int GOLD_TIMER_START_VALUE = 0x26;

	// ���ɗ��������ɗh���A�j���[�V�����̃X�^�[�g�t���[��
	static constexpr int HOLE_ANIMATION_START_FRAME = 11;

	// ���ɗ��������ɉ��ɗh���A�j���[�V�����̃e�[�u���T�C�Y
	static constexpr int HOLE_ANIMATION_TABLE_SIZE = 5;

	// ���ɗ��������ɉ��ɗh���A�j���[�V�����e�[�u���iX�̗h�ꕝ�j
	static constexpr int HOLE_ANIMATION_TABLE[HOLE_ANIMATION_TABLE_SIZE] = { 2, 1, 2, 3, 2 };

	// �����A�j���[�V�����p
	static constexpr int RESURRECTION_ANIME_TIME_NONE = 20;
	static constexpr int RESURRECTION_ANIME_TIME_EGG01 = 19;
	static constexpr int RESURRECTION_ANIME_TIME_EGG02 = 10;

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �Q�[���v���C�V�[���ւ̃|�C���^
	GamePlayScene* m_pGamePlayScene;

	// �X�e�[�W�ւ̃|�C���^
	Level* m_pLevel;

	// �A�N�e�B�u�Ȃ�true��ݒ�
	bool m_isActive;

	// �����t���O
	bool m_isAlive;

	// �\���t���O
	bool m_isDisplay;

	// �^�C����̈ʒu
	POINT m_tilePosition;

	// �^�C�����̒����p�̈ʒu
	POINT m_adjustPosition;

	// ��̌���
	Direction m_faceDirection;

	// �A�j���[�V�����X�e�[�g
	GuradAnimationState m_guardAnimationState;

	// ����ێ��^�C�}�[
	int m_goldTimer;

	// �����^�C�}�[
	int m_resurrectionTimer;

	// �ړ�����
	MoveDirection m_moveDirection;

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	Gurad(GamePlayScene* pScene, Level* pLevel);

	// �f�X�g���N�^
	~Gurad();

	// �������֐�
	void Initialize(POINT tilePosition, POINT ajustPosition);

	// �X�V����
	void Update();

	// �`��֐�
	void Render(int ghTileset);

	// �A�N�e�B�u��ݒ肷��֐�
	void SetActive(bool active) { m_isActive = active; }

	// �A�N�e�B�u�����ׂ�֐�
	bool IsActive() const { return m_isActive; }

	// �����Ă��邩���ׂ�֐�
	bool IsAlive() const { return m_isAlive; }

	// �^�C����̈ʒu��ݒ肷��֐�
	void SetTilePosition(int colmun, int row)
	{
		m_tilePosition.x = colmun;
		m_tilePosition.y = row;
		SetAdjustPosition(Level::TILE_CENTER_X, Level::TILE_CENTER_Y);
	}

	// �^�C����̈ʒu���擾����֐�
	POINT GetTilePosition() const { return m_tilePosition; }

	// �^�C�����̈ʒu��ݒ肷��֐�
	void SetAdjustPosition(int x, int y)
	{
		m_adjustPosition.x = x;
		m_adjustPosition.y = y;
	}

	// �^�C�����̈ʒu���擾����֐�
	POINT GetAdjustPosition() const { return m_adjustPosition; }

	// ����ێ��^�C�}�[��ݒ肷��֐�
	void SetGoldTimer(int goldTimer) { m_goldTimer = goldTimer; }

	// ����ێ��^�C�}�[���擾����֐�
	int GetGoldTimer() const { return m_goldTimer; }

	// �����^�C�}�[�̐ݒ�֐�
	void SetResurrectionTimer(int timer) { m_resurrectionTimer = timer; }

	// �����^�C�}�[�̎擾�֐�
	int GetResurrectionTimer() const { return m_resurrectionTimer; }

	// �w��ʒu���畜������֐�
	void Resurrection(int colmun, int row);

private:

	// �ʒu���ɒ�������֐�
	void AjustCloumn();

	// �ʒu���s�ɒ�������֐�
	void AjustRow();

	// �������Ă��邩�`�F�b�N����֐�
	bool IsFalling();

	// ��Ɉړ��\�����ׂ�֐�
	bool IsMovableUp();

	// ���Ɉړ��\�����ׂ�֐�
	bool IsMovableDown();

	// ���Ɉړ��\�����ׂ�֐�
	bool IsMovableLeft();

	// �E�Ɉړ��\�����ׂ�֐�
	bool IsMovableRight();

	// ������
	void Falling();

	// �ړ����������߂�֐�
	MoveDirection DecideMoveDirection();

	// ��Ɉړ�
	void MoveUp();

	// ���Ɉړ�
	void MoveDown();

	// ���Ɉړ�
	void MoveLeft();

	// �E�Ɉړ�
	void MoveRight();

	// �K�[�h�A�j���[�V�����X�e�[�g�̐ݒ�
	void SetGuradAnimationState(GuradAnimationState start, GuradAnimationState end);

	// ���򂪏E���邩���ׂ�֐�
	void CheckGoldPickedUp();

	// �w�肵���s�܂ňړ��\�����ׂ�֐�
	bool IsMovableColumn(int column);
	
	// ���E�ǂ��܂ňړ��ł��邩�擾����֐�
	void GetLeftRightLimits(int* colmun, Direction direction, int limit);

	// �������Ɉړ��ȍœK�ȍs��������֐�
	int FindCandidateRowBelow(int colmun, int row);

	// ������Ɉړ��ȍœK�ȍs��������֐�
	int FindCandidateRowAbove(int colmun, int row);

	// ���E�Ɉړ��ł����������ׂ�֐�
	bool CheckLeftRightMove(int colmun, int row, Direction direction);

	// �v���C���[�Ƃ̋^�����������߂�֐�
	int GetPsuedoDistance(int row);

	// �㉺�ֈړ����邩���ׂ�֐�
	Gurad::MoveDirection SelectMoveUpAndDown(int* bestGuradDistance);

	// ���ֈړ����邩���ׂ�֐�
	Gurad::MoveDirection SelectMoveLeftAndRight(int* bestGuradDistance, int leftColmunLimit, Direction direction);

	// ���Ɉړ��\�ȃ^�C�������ׂ�֐�
	bool IsMovableDown(int colmun, int row);

	// ��Ɉړ��\�ȃ^�C�������ׂ�֐�
	bool IsMovableUp(int colmun, int row);

	// �ړ��\�ȃ^�C�������ׂ�֐��i�����j
	bool IsMovableTileFall(Level::Tile page1, Level::Tile page2);

	// �ړ��\�ȃ^�C�������ׂ�֐��i�㍶�E�j
	bool IsMovableTileULR(Level::Tile tile);

	// �ړ��\�ȃ^�C�������ׂ�֐��i���j
	bool IsMovableTileD(Level::Tile tile);

	// ����ێ��^�C�}�[���X�V���ċ���𗎂Ƃ�����
	void UpdateGoldDropTimer();

};
