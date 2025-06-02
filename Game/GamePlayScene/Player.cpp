//--------------------------------------------------------------------------------------
// File: Player.cpp
//
//--------------------------------------------------------------------------------------
#include "Player.h"
#include "Game/Tile.h"

// �R���X�g���N�^
Player::Player()
	: m_isActive{ false }
	, m_tilePosition{ 0, 0 }
	, m_ajustPosition{ 0, 0 }
	, m_faceDirection{ FaceDirection::Right }
	, m_animationState{ AnimationState::Run01_R }
{
}

// �f�X�g���N�^
Player::~Player()
{
}

// ����������
void Player::Initialize(POINT tilePosition, POINT ajustPosition)
{
	// �A�N�e�B�u�ɂ���
	m_isActive = true;

	// �ʒu������������
	m_tilePosition = tilePosition;
	m_ajustPosition = ajustPosition;

	// �v���C���[�̌���
	m_faceDirection = FaceDirection::Right;
}

// �X�V����
void Player::Update(int keyCondition, int keyTrigger, Stage* pStage)
{
	// �ړ��\����Ȃ�
	if (!IsMovable(pStage))
	{
		// ��ɍ��킹��悤��X���W�𒲐�����
		AjustCloumn();

		// ����
		m_ajustPosition.y++;
		if (m_ajustPosition.y > 4)
		{
			m_ajustPosition.y = 0;
			m_tilePosition.y++;
		}

		// �����A�j���[�V����
		if (m_faceDirection == FaceDirection::Left)
		{
			m_animationState = AnimationState::Fall_L;
		}
		else
		{
			m_animationState = AnimationState::Fall_R;
		}
		return;
	}

	// ��L�[�������ꂽ
	if (keyCondition & PAD_INPUT_UP)
	{
		// ��Ɉړ��\���H
		if (IsMovableUp(pStage))
		{
			// ��ɍ��킹��悤��X���W�𒲐�����
			AjustCloumn();

			// ��Ɉړ�����
			m_ajustPosition.y--;
			if (m_ajustPosition.y < 0)
			{
				m_ajustPosition.y = 4;
				m_tilePosition.y--;
			}

			// �n�V�S��o��A�j���[�V����
			SetAnimationState(AnimationState::Climb01, AnimationState::Climb02);
		}
	}

	// ���L�[�������ꂽ
	if (keyCondition & PAD_INPUT_DOWN)
	{
		// ���Ɉړ��\���H
		if (IsMovableDown(pStage))
		{
			// ��ɍ��킹��悤��X���W�𒲐�����
			AjustCloumn();

			// ���Ɉړ�����
			m_ajustPosition.y++;
			if (m_ajustPosition.y > 4)
			{
				m_ajustPosition.y = 0;
				m_tilePosition.y++;
			}

			// �n�V�S��o��A�j���[�V����
			SetAnimationState(AnimationState::Climb01, AnimationState::Climb02);
		}
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
			m_ajustPosition.x--;
			if (m_ajustPosition.x < 0)
			{
				m_ajustPosition.x = 4;
				m_tilePosition.x--;
			}

			// ���[�v�̏ꍇ
			if (pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Rope)
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
			m_ajustPosition.x++;
			if (m_ajustPosition.x > 4)
			{
				m_ajustPosition.x = 0;
				m_tilePosition.x++;
			}

			// ���[�v�̏ꍇ
			if (pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Rope)
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
	DrawRectGraph( m_tilePosition.x * Tile::TILE_PIXEL_WIDTH + (m_ajustPosition.x - Tile::TILE_CENTER_X) * 2
				 , m_tilePosition.y * Tile::TILE_PIXEL_HEIGHT + (m_ajustPosition.y - Tile::TILE_CENTER_Y) * 2
				 , Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				 , Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
}

// �ʒu���ɒ�������֐�
void Player::AjustCloumn()
{
	// ��������K�v�͂Ȃ�
	if (m_ajustPosition.x == Tile::TILE_CENTER_X) return;

	// ���E�ɂ���Ă���ꍇ�͒�������
	if (m_ajustPosition.x < Tile::TILE_CENTER_X)
	{
		m_ajustPosition.x++;
	}
	else
	{
		m_ajustPosition.x--;
	}
}

// �ʒu���s�ɒ�������֐�
void Player::AjustRow()
{
	// ��������K�v�͂Ȃ�
	if (m_ajustPosition.y == Tile::TILE_CENTER_Y) return;

	// �㉺�ɂ���Ă���ꍇ�͒�������
	if (m_ajustPosition.y < Tile::TILE_CENTER_Y)
	{
		m_ajustPosition.y++;
	}
	else
	{
		m_ajustPosition.y--;
	}
}

// �ړ��\�����ׂ�֐�
bool Player::IsMovable(Stage* pStage) const
{
	// �n�V�S�Ȃ�ړ��\
	if (pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Ladder) return true;

	// ���[�v�ɂ���ł���Έړ��\
	if ( (pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Rope)
	  && (m_ajustPosition.y == Tile::TILE_CENTER_Y)
	   )
	{
		return true;
	}

	// �󒆂ɕ����Ă���
	if (m_ajustPosition.y < Tile::TILE_CENTER_Y) return false;

	// ��ԉ��Ȃ�ړ��\
	if (m_tilePosition.y == Stage::STAGE_HEIGHT - 1) return true;

	// ���̍s�������\�Ȃ�
	if (Tile::IsMovableTileFall(pStage->GetTileType(m_tilePosition.x, m_tilePosition.y + 1))) return false;

	return true;
}

// ��Ɉړ��\�����ׂ�֐�
bool Player::IsMovableUp(Stage* pStage) const
{
	// �n�V�S������H
	if (pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Ladder)
	{
		// �n�V�S��Ȃ̂Ńn�V�S�̐^�񒆂܂ł͈ړ��\
		if (m_ajustPosition.y > Tile::TILE_CENTER_Y) return true;

		// ��ԏ�Ȃ�ړ��s��
		if (m_tilePosition.y == 0) return false;

		// ��̍s���ړ��\�Ȃ�
		if (Tile::IsMovableTileULR(pStage->GetTileType(m_tilePosition.x, m_tilePosition.y - 1))) return true;
	}
	else
	{
		if (m_ajustPosition.y > Tile::TILE_CENTER_Y) return true;
	}
	return false;
}

bool Player::IsMovableDown(Stage* pStage) const
{
	// ������ɂ���̂ŉ��Ɉړ��\
	if (m_ajustPosition.y < Tile::TILE_CENTER_Y) return true;

	// ��ԉ��Ȃ�ړ��s��
	if (m_tilePosition.y == Stage::STAGE_HEIGHT - 1) return false;

	// ���̍s���ړ��\�Ȃ�
	if (Tile::IsMovableTileDown(pStage->GetTileType(m_tilePosition.x, m_tilePosition.y + 1))) return true;

	return false;
}

// ���Ɉړ��\�����ׂ�֐�
bool Player::IsMovableLeft(Stage* pStage) const
{
	// �܂��������Ɉړ�������]�n������
	if (m_ajustPosition.x > Tile::TILE_CENTER_X) return true;

	// �����Ȃ̂ňړ��s��
	if (m_tilePosition.x == 0) return false;

	// �ړ���^�C�����ړ��\�����ׂ�
	return Tile::IsMovableTileULR(pStage->GetTileType(m_tilePosition.x - 1, m_tilePosition.y));
}

// �E�Ɉړ��\�����ׂ�֐�
bool Player::IsMovableRight(Stage* pStage) const
{
	// �܂��E�����Ɉړ�������]�n������
	if (m_ajustPosition.x < Tile::TILE_CENTER_X) return true;

	// �E���Ȃ̂ňړ��s��
	if (m_tilePosition.x == Stage::STAGE_WIDTH - 1) return false;

	// �ړ���^�C�����ړ��\�����ׂ�
	return Tile::IsMovableTileULR(pStage->GetTileType(m_tilePosition.x + 1, m_tilePosition.y));
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
