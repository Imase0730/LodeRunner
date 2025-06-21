//--------------------------------------------------------------------------------------
// File: Player.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Level.h"
#include "Game/Tile.h"

class GamePlayScene;

// �v���C���[�N���X
class Player
{
public:

	// ����
	enum class Direction
	{
		Right,	// �E
		Left,	// ��
	};

	// �A�j���[�V�����X�e�[�g
	enum class PlayerAnimationState
	{
		AnimNone,						// �\�����Ȃ�
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

	// �@��A�j���[�V�����X�e�[�g
	enum class DigAnimationState
	{
		NotDigging = -1,
		Dig01, Dig02, Dig03, Dig04, Dig05, Dig06,
		Dig07, Dig08, Dig09, Dig10, Dig11, Dig12,
	};

	// �N���X�萔�̐錾 -------------------------------------------------
private:

	// �v���C���[�̊G�̈ʒu
	static constexpr POINT PLAYER_SPRITES[] =
	{
		// �\�����Ȃ�
		{ 0, 4 },

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

	// �@���Ă��郌���K�̊G�̈ʒu
	static constexpr POINT DIG_BRICK_SPRITES[] =
	{
		{ 0, 10 }, { 0, 10 },
		{ 1, 10 }, { 1, 10 },
		{ 2, 10 }, { 2, 10 },
		{ 3, 10 }, { 3, 10 },
		{ 4, 10 }, { 4, 10 },
		{ 5, 10 }, { 5, 10 },
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

	// �Q�[���v���C�V�[���ւ̃|�C���^
	GamePlayScene* m_pGamePlayScene;

	// ���x���ւ̃|�C���^
	Level* m_pLevel;

	// �A�N�e�B�u�Ȃ�true��ݒ�
	bool m_isActive;

	// �����t���O
	bool m_isAlive;

	// �\���t���O
	bool m_isVisible;

	// �^�C����̈ʒu
	POINT m_tilePosition;

	// �^�C�����̒����p�̈ʒu
	POINT m_adjustPosition;

	// ��̌���
	Direction m_faceDirection;

	// �A�j���[�V�����X�e�[�g
	PlayerAnimationState m_playerAnimationState;

	// �@�����
	DigDirection m_digDirection;

	// �@��A�j���[�V�����X�e�[�g
	DigAnimationState m_digAnimationState;

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	Player(GamePlayScene* pScene, Level* pLevel);

	// �f�X�g���N�^
	~Player();

	// �������֐�
	void Initialize(POINT tilePosition, POINT ajustPosition);

	// �X�V����
	void Update(int keyCondition, int keyTrigger);

	// �`��֐�
	void Render(int ghTileset) const;

	// �A�N�e�B�u��ݒ肷��֐�
	void SetActive(bool active) { m_isActive = active; }

	// �A�N�e�B�u�����ׂ�֐�
	bool IsActive() const { return m_isActive; }

	// �����Ă��邩�ݒ肷��֐�
	void SetAlive(bool alive);

	// �������Ă��邩���ׂ�֐�
	bool IsAlive() const { return m_isAlive; }

	// �\��(ON/OFF)
	void SetVisible(bool isVisible) { m_isVisible = isVisible; }

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
	bool IsFalling();

	// ��Ɉړ��\�����ׂ�֐�
	bool IsMovableUp();

	// ���Ɉړ��\�����ׂ�֐�
	bool IsMovableDown();

	// ���Ɍ@��邩���ׂ�֐�
	bool IsDiggableLeft();

	// �E�Ɍ@��邩���ׂ�֐�
	bool IsDiggableRight();

	// ���Ɉړ��\�����ׂ�֐�
	bool IsMovableLeft();

	// �E�Ɉړ��\�����ׂ�֐�
	bool IsMovableRight();

	// ���Ɍ@��
	void DigLeft();

	// �E�Ɍ@��
	void DigRight();

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

	// �v���C���[�A�j���[�V�����X�e�[�g�̐ݒ�
	void SetPlayerAnimationState(PlayerAnimationState start, PlayerAnimationState end);

	// �@��A�j���[�V�����̍X�V
	void UpdateDigAnimation();

	// ���򂪏E���邩���ׂ�֐�
	void CheckGoldPickedUp();

	// �ړ��\�ȃ^�C�������ׂ�֐��i�����j
	bool IsMovableTileFall(Tile::Type page1, Tile::Type page2);

	// �ړ��\�ȃ^�C�������ׂ�֐��i�㍶�E�j
	static bool IsMovableTileULR(Tile::Type tile);

	// �ړ��\�ȃ^�C�������ׂ�֐��i���j
	static bool IsMovableTileDown(Tile::Type tile);

};
