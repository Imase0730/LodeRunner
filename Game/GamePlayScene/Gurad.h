//--------------------------------------------------------------------------------------
// File: Gurad.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Stage.h"

class GamePlayScene;
class Stage;

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
		Run01_L, Run02_L, Run03_L,		// ����i�������j
		Rope01_L, Rope02_L, Rope03_L,	// ���[�v�ňړ��i�������j
		Fall_L,							// �������i�������j

		Run01_R, Run02_R, Run03_R,		// ����i�E�����j
		Rope01_R, Rope02_R, Rope03_R,	// ���[�v�ňړ��i�E�����j
		Fall_R,							// �������i�E�����j

		Climb01, Climb02,				// �n�V�S��o����
	};

	// �N���X�萔�̐錾 -------------------------------------------------
private:

	// �K�[�h�̊G�̈ʒu
	static constexpr POINT GUARD_SPRITES[] =
	{
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
	};

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �Q�[���v���C�V�[���ւ̃|�C���^
	GamePlayScene* m_pGamePlayScene;

	// �X�e�[�W�ւ̃|�C���^
	Stage* m_pStage;

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
	Gurad(GamePlayScene* pScene, Stage* pStage);

	// �f�X�g���N�^
	~Gurad();

	// �������֐�
	void Initialize(POINT tilePosition, POINT ajustPosition);

	// �X�V����
	void Update();

	// �`��֐�
	void Render(int ghTileset) const;

	// �A�N�e�B�u��ݒ肷��֐�
	void SetActive(bool active) { m_isActive = active; }

	// �A�N�e�B�u�����ׂ�֐�
	bool IsActive() const { return m_isActive; }

	// �������Ă��邩���ׂ�֐�
	bool IsAlive() const { return m_isAlive; }

	// �^�C����̈ʒu���擾����֐�
	POINT GetTilePosition() const { return m_tilePosition; }

	// �^�C�����̈ʒu���擾����֐�
	POINT GetAdjustPosition() const { return m_adjustPosition; }

private:

	// �ʒu���ɒ�������֐�
	void AjustCloumn();

	// �ʒu���s�ɒ�������֐�
	void AjustRow();

	// �������Ă��邩�`�F�b�N����֐�
	bool IsFalling() const;

	// ��Ɉړ��\�����ׂ�֐�
	bool IsMovableUp() const;

	// ���Ɉړ��\�����ׂ�֐�
	bool IsMovableDown() const;

	// ���Ɉړ��\�����ׂ�֐�
	bool IsMovableLeft() const;

	// �E�Ɉړ��\�����ׂ�֐�
	bool IsMovableRight() const;

	// ������
	void Falling();

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
	bool IsMovableColumn(int column) const;
	
	// ���E�ǂ��܂ňړ��ł��邩�擾����֐�
	void GetLeftRightLimits(int* colmun, Direction direction, int limit);

	// �������Ɉړ��ȍœK�ȍs��������֐�
	int FindCandidateRowBelow(int colmun, int row);

	// ������Ɉړ��ȍœK�ȍs��������֐�
	int FindCandidateRowAbove(int colmun, int row);

	// ���E�Ɉړ��ł����������ׂ�֐�
	bool CheckLeftRightMove(int colmun, int row, Direction direction);

	// �v���C���[�Ƃ̋^�����������߂�֐�
	int GetPseudoDistance(int colmun, int row);

	// �㉺�ֈړ����邩���ׂ�֐�
	void SelectMoveUpAndDown(int* bestGuradDistance);

	// ���ֈړ����邩���ׂ�֐�
	void SelectMoveLeftAndRight(int* bestGuradDistance, int leftColmunLimit, Direction direction);

	// ���Ɉړ��\�ȃ^�C�������ׂ�֐�
	bool IsMovableDown(int colmun, int row);

	// ��Ɉړ��\�ȃ^�C�������ׂ�֐�
	bool IsMovableUp(int colmun, int row);

};
