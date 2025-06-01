//--------------------------------------------------------------------------------------
// File: Player.cpp
//
//--------------------------------------------------------------------------------------
#include "Player.h"
#include "Game/Tile.h"

// �R���X�g���N�^
Player::Player()
	: m_isActive{ false }
	, m_position{ 0, 0 }
	, m_faceDirection{ FaceDirection::Right }
	, m_animationState{ AnimationState::Run01_R }
{
}

// �f�X�g���N�^
Player::~Player()
{
}

// ����������
void Player::Initialize(POINT position)
{
	// �A�N�e�B�u�ɂ���
	m_isActive = true;

	// �ʒu������������
	m_position = position;

	// �v���C���[�̌���
	m_faceDirection = FaceDirection::Right;
}

// �X�V����
void Player::Update(int keyCondition, int keyTrigger, Stage* pStage)
{
	// ��L�[�������ꂽ
	if (keyCondition & PAD_INPUT_UP)
	{
		// ��Ɉړ��\���H
		if (IsMovableUp(pStage))
		{
			// ��ɍ��킹��悤��X���W�𒲐�����
			AjustCloumn();

			// ��Ɉړ�����
			m_position.y -= 2;

			// �n�V�S��o��A�j���[�V����
			SetAnimationState(AnimationState::Climb01, AnimationState::Climb02);
		}
	}
	// ���L�[�������ꂽ
	if (keyCondition & PAD_INPUT_DOWN)
	{

	}
	// ���L�[�������ꂽ
	if (keyCondition & PAD_INPUT_LEFT)
	{
		// ���Ɉړ��\���H
		if (IsMovableLeft(pStage))
		{
			// ��������
			m_faceDirection = FaceDirection::Left;
			
			// �s�ɍ��킹��悤��Y���W�𒲐�����
			AjustRow();

			// ���Ɉړ�����
			m_position.x -= 2;

			// �v���C���[�̈ʒu�̃^�C�����擾
			int column = m_position.x / Tile::TILE_WIDTH;
			int row = m_position.y / Tile::TILE_HEIGHT;
	
			// ���[�v�̏ꍇ
			if (pStage->GetTileType(column, row) == Tile::TileType::Rope)
			{
				// ���[�v�ňړ�����A�j���[�V����
				SetAnimationState(AnimationState::Rope01_L, AnimationState::Rope03_L);
			}
			else
			{
				// ����A�j���[�V����
				SetAnimationState(AnimationState::Run01_L, AnimationState::Run03_L);
			}
		}
	}

	// �E�L�[�������ꂽ
	if (keyCondition & PAD_INPUT_RIGHT)
	{
		// �E�Ɉړ��\���H
		if (IsMovableRight(pStage))
		{
			// �E������
			m_faceDirection = FaceDirection::Right;

			// �s�ɍ��킹��悤��Y���W�𒲐�����
			AjustRow();

			// �E�ړ�����
			m_position.x += 2;

			// �v���C���[�̈ʒu�̃^�C�����擾
			int column = m_position.x / Tile::TILE_WIDTH;
			int row = m_position.y / Tile::TILE_HEIGHT;

			// ���[�v�̏ꍇ
			if (pStage->GetTileType(column, row) == Tile::TileType::Rope)
			{
				// ���[�v�ňړ�����A�j���[�V����
				SetAnimationState(AnimationState::Rope01_R, AnimationState::Rope03_R);
			}
			else
			{
				// ����A�j���[�V����
				SetAnimationState(AnimationState::Run01_R, AnimationState::Run03_R);
			}
		}
	}
}

// �`�揈��
void Player::Render(int ghTileset) const
{
	POINT pos = SPRITE_POSITION[static_cast<int>(m_animationState)];
	DrawRectGraph(m_position.x - Tile::CENTER_OFFSET_X, m_position.y - Tile::CENTER_OFFSET_Y
		, Tile::TILE_WIDTH * pos.x, Tile::TILE_HEIGHT * pos.y
		, Tile::TILE_WIDTH, Tile::TILE_HEIGHT, ghTileset, TRUE);
}

// �ʒu���ɒ�������֐�
void Player::AjustCloumn()
{
	int cloumn = m_position.x / Tile::TILE_WIDTH;
	int x = m_position.x % Tile::TILE_WIDTH;

	// ��������K�v�͂Ȃ�
	if (x == Tile::CENTER_OFFSET_X) return;

	// ���E�ɂ���Ă���ꍇ�͒�������
	if (x < Tile::CENTER_OFFSET_X)
	{
		x += 2;
		if (x > Tile::CENTER_OFFSET_X) x = Tile::CENTER_OFFSET_X;
	}
	else
	{
		x -= 2;
		if (x < Tile::CENTER_OFFSET_X) x = Tile::CENTER_OFFSET_X;
	}
	m_position.x = cloumn * Tile::TILE_WIDTH + x;
}

// �ʒu���s�ɒ�������֐�
void Player::AjustRow()
{
	int row = m_position.y / Tile::TILE_HEIGHT;
	int y = m_position.y % Tile::TILE_HEIGHT;

	// ��������K�v�͂Ȃ�
	if (y == Tile::CENTER_OFFSET_Y) return;

	// �㉺�ɂ���Ă���ꍇ�͒�������
	if (y < Tile::CENTER_OFFSET_Y)
	{
		y += 2;
		if (y > Tile::CENTER_OFFSET_Y) y = Tile::CENTER_OFFSET_Y;
	}
	else
	{
		y -= 2;
		if (y < Tile::CENTER_OFFSET_Y) y = Tile::CENTER_OFFSET_Y;
	}
	m_position.y = row * Tile::TILE_HEIGHT + y;
}

// ��Ɉړ��\�����ׂ�֐�
bool Player::IsMovableUp(Stage* pStage) const
{
	// �v���C���[�̈ʒu�̃^�C�����擾
	int column = m_position.x / Tile::TILE_WIDTH;
	int row = m_position.y / Tile::TILE_HEIGHT;
	int y = m_position.y % Tile::TILE_HEIGHT;

	// �n�V�S������H
	if (pStage->GetTileType(column, row) == Tile::TileType::Ladder)
	{
		// �n�V�S��Ȃ̂Ńn�V�S�̐^�񒆂܂ł͈ړ��\
		if (y > Tile::CENTER_OFFSET_Y) return true;

		// ��ԏ�Ȃ�ړ��s��
		if (row == 0) return false;

		// ��̍s���ړ��\�Ȃ�
		if (Tile::IsMovableTile(pStage->GetTileType(column, row - 1))) return true;
	}
	// �������n�V�S�̏ꍇ�͏�܂œo�邱�Ƃ��\
	else if (pStage->GetTileType(column, row + 1) == Tile::TileType::Ladder)
	{
		if (y > Tile::CENTER_OFFSET_Y) return true;
	}
	return false;
}

// ���Ɉړ��\�����ׂ�֐�
bool Player::IsMovableLeft(Stage* pStage) const
{
	// �܂��������Ɉړ�������]�n������
	if ((m_position.x % Tile::TILE_WIDTH) > Tile::CENTER_OFFSET_X) return true;

	int column = m_position.x / Tile::TILE_WIDTH;
	int row = m_position.y / Tile::TILE_HEIGHT;

	// �����Ȃ̂ňړ��s��
	if (column == 0) return false;

	column--;

	// �ړ���^�C�����ړ��\�����ׂ�
	return Tile::IsMovableTile(pStage->GetTileType(column, row));
}

// �E�Ɉړ��\�����ׂ�֐�
bool Player::IsMovableRight(Stage* pStage) const
{
	// �܂��E�����Ɉړ�������]�n������
	if ((m_position.x % Tile::TILE_WIDTH) < Tile::CENTER_OFFSET_X) return true;

	int column = m_position.x / Tile::TILE_WIDTH;
	int row = m_position.y / Tile::TILE_HEIGHT;

	// �E���Ȃ̂ňړ��s��
	if (column == Stage::STAGE_WIDTH - 1) return false;

	column++;

	// �ړ���^�C�����ړ��\�����ׂ�
	return Tile::IsMovableTile(pStage->GetTileType(column, row));
}

// �A�j���[�V�����X�e�[�g�̐ݒ�
void Player::SetAnimationState(AnimationState beginAnimState, AnimationState endAnimState)
{
	int begin = static_cast<int>(beginAnimState);
	int end = static_cast<int>(endAnimState);
	int animState = static_cast<int>(m_animationState);

	// ���̃A�j���[�V�����֍Ō�܂ŏI�������n�߂ɖ߂�
	animState++;

	// �ʂ̃A�j���[�V�����ֈڍs�̏ꍇ�͎n�߂���
	if (animState < begin)
	{
		animState = begin;
	}
	else if (animState > end)
	{
		animState = begin;
	}

	m_animationState = static_cast<AnimationState>(animState);
}
