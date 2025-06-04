//--------------------------------------------------------------------------------------
// File: Player.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Stage.h"

// �v���C���[�N���X
class Player
{
public:

	// ����
	enum class FaceDirection
	{
		Right,	// �E
		Left,	// ��
	};

	// �A�j���[�V�����C���f�b�N�X
	enum class PlayerAnimationState
	{
		Run01_L, Run02_L, Run03_L,		// ����i�������j
		Rope01_L, Rope02_L, Rope03_L,	// ���[�v�ňړ��i�������j
		Dig_L,							// ���@��i�������j
		Fall_L,							// �������i�������j

		Run01_R, Run02_R, Run03_R,		// ����i�E�����j
		Rope01_R, Rope02_R, Rope03_R,	// ���[�v�ňړ��i�E�����j
		Dig_R,							// ���@��i�E�����j
		Fall_R,							// �������i�E�����j

		Climb01, Climb02,				// �n�V�S��o����
	};

	// �@���Ă������
	enum class DigDirection
	{
		NotDigging,	// �@���Ă��Ȃ�
		Right,		// �E
		Left,		// ��
	};

	// �N���X�萔�̐錾 -------------------------------------------------
private:

	// �v���C���[�̊G�̈ʒu
	static constexpr POINT PLAYER_SPRITES[] =
	{
		// ����i�������j
		{ 0, 5 },
		{ 1, 5 },
		{ 2, 5 },

		// ���[�v�ňړ��i�������j
		{ 0, 6 },
		{ 1, 6 },
		{ 2, 6 },

		// ���@��i�������j
		{ 0, 9 },

		// �������i�������j
		{ 8, 5 },

		// ����i�E�����j
		{ 3, 5 },
		{ 4, 5 },
		{ 5, 5 },

		// ���[�v�ňړ��i�E�����j
		{ 3, 6 },
		{ 4, 6 },
		{ 5, 6 },

		// ���@��i�E�����j
		{ 5, 9 },

		// �������i�E�����j
		{ 9, 5 },

		// �n�V�S������Ă���
		{ 6, 5 },
		{ 7, 5 },
	};

	// �@���Ă���Œ��̃����K�̔j�Ђ̊G�̈ʒu�i�������j
	static constexpr POINT DIG_DEBRIS_LEFT_SPRITES[] =
	{
		{ 1, 9 }, { 1, 9 },
		{ 2, 9 }, { 2, 9 },
		{ 3, 9 }, { 3, 9 },
		{ 4, 9 }, { 4, 9 },
		{ -1, -1 }, { -1, -1 },
		{ -1, -1 }, { -1, -1 },
	};

	// �@���Ă���Œ��̃����K�̔j�Ђ̊G�̈ʒu�i�E�����j
	static constexpr POINT DIG_DEBRIS_RIGHT_SPRITES[] =
	{
		{ 6, 9 }, { 6, 9 },
		{ 7, 9 }, { 7, 9 },
		{ 8, 9 }, { 8, 9 },
		{ 9, 9 }, { 9, 9 },
		{ -1, -1 }, { -1, -1 },
		{ -1, -1 }, { -1, -1 },
	};

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �A�N�e�B�u�Ȃ�true��ݒ�
	bool m_isActive;

	// �^�C����̈ʒu
	POINT m_tilePosition;

	// �^�C�����̒����p�̈ʒu
	POINT m_ajustPosition;

	// ��̌���
	FaceDirection m_faceDirection;

	// �A�j���[�V�����X�e�[�g
	PlayerAnimationState m_playerAnimationState;

	// �@�����
	DigDirection m_digDirection;

	// �@��A�j���[�V�����X�e�[�g
	Tile::DigAnimationState m_digAnimationState;

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	~Player();

	// �������֐�
	void Initialize(POINT tilePosition, POINT ajustPosition);

	// �X�V����
	void Update(int keyCondition, int keyTrigger, Stage* pStage);

	// �`��֐�
	void Render(int ghTileset) const;

	// �A�N�e�B�u��ݒ肷��֐�
	void SetActive(bool active) { m_isActive = active; }

	// �A�N�e�B�u�����ׂ�֐�
	bool IsActive() const { return m_isActive; }

private:

	// �ʒu���ɒ�������֐�
	void AjustCloumn();

	// �ʒu���s�ɒ�������֐�
	void AjustRow();

	// �ړ��\�����ׂ�֐�
	bool IsMovable(Stage* pStage) const;

	// ��Ɉړ��\�����ׂ�֐�
	bool IsMovableUp(Stage* pStage) const;

	// ���Ɉړ��\�����ׂ�֐�
	bool IsMovableDown(Stage* pStage) const;

	// ���Ɍ@��邩���ׂ�֐�
	bool IsDiggableLeft(Stage* pStage) const;

	// ���Ɉړ��\�����ׂ�֐�
	bool IsMovableLeft(Stage* pStage) const;

	// �E�Ɉړ��\�����ׂ�֐�
	bool IsMovableRight(Stage* pStage) const;

	// �v���C���[�A�j���[�V�����X�e�[�g�̐ݒ�
	void SetPlayerAnimationState(PlayerAnimationState start, PlayerAnimationState end);

};
