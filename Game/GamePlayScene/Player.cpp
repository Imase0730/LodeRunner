//--------------------------------------------------------------------------------------
// File: Player.cpp
//
//--------------------------------------------------------------------------------------
#include "Player.h"
#include "Game/Tile.h"

// �R���X�g���N�^
Player::Player()
	: m_isActive{ false }
	, m_isAlive{ false }
	, m_isDisplay{ false }
	, m_tilePosition{ 0, 0 }
	, m_ajustPosition{ 0, 0 }
	, m_faceDirection{ FaceDirection::Right }
	, m_playerAnimationState{ PlayerAnimationState::Run01_R }
	, m_digDirection{ DigDirection::NotDigging }
	, m_digAnimationState{ Tile::DigAnimationState::NotDigging }
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

	// �����t���O������
	m_isAlive = true;

	// �\��(ON)
	m_isDisplay = true;

	// �ʒu������������
	m_tilePosition = tilePosition;
	m_ajustPosition = ajustPosition;

	// �e�ϐ��̏�����
	m_faceDirection = FaceDirection::Right;
	m_playerAnimationState = PlayerAnimationState::Run01_R;
	m_digDirection = DigDirection::NotDigging;
	m_digAnimationState = Tile::DigAnimationState::NotDigging;
}

// �X�V����
void Player::Update(int keyCondition, int keyTrigger, Stage* pStage)
{
	// ������ꏊ�������K�Ȃ玀�S
	if (pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Blick)
	{
		// ���S
		m_isAlive = false;
		// �\��(OFF)
		m_isDisplay = false;
	}

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
			m_playerAnimationState = PlayerAnimationState::Fall_L;
		}
		else
		{
			m_playerAnimationState = PlayerAnimationState::Fall_R;
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
			SetPlayerAnimationState(PlayerAnimationState::Climb01, PlayerAnimationState::Climb02);
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
			SetPlayerAnimationState(PlayerAnimationState::Climb01, PlayerAnimationState::Climb02);
		}
	}

	// Z�L�[�������ꂽ���@���Ă��Ȃ��Ȃ獶�����@��
	if ((keyTrigger & PAD_INPUT_1) && (m_digDirection == DigDirection::NotDigging))
	{
		// �������Ɍ@��
		m_digDirection = DigDirection::Left;
		// �@��A�j���[�V�����ݒ�
		m_digAnimationState = Tile::DigAnimationState::Dig01;
	}

	// �������Ɍ@���Ă���Œ�
	if (m_digDirection == DigDirection::Left)
	{
		// �������@�邱�Ƃ��\���H
		if (IsDiggableLeft(pStage))
		{
			// ��ɍ��킹��悤��X���W�𒲐�����
			AjustCloumn();
			// �s�ɍ��킹��悤��Y���W�𒲐�����
			AjustRow();

			// �v���C���[�̃A�j���[�V������ݒ�i�@���Ă���r���ŊG��ς��Ă���j
			if (m_digAnimationState == Tile::DigAnimationState::Dig01) m_playerAnimationState = PlayerAnimationState::Dig_L;
			if (m_digAnimationState == Tile::DigAnimationState::Dig06) m_playerAnimationState = PlayerAnimationState::Run01_L;

			// �X�e�[�W���̃����K�̌@��A�j���[�V�����̍X�V
			pStage->SetTileDigAnimationState(m_tilePosition.x - 1, m_tilePosition.y + 1, m_digAnimationState);

			// �@��I������H
			if (m_digAnimationState == Tile::DigAnimationState::NotDigging)
			{
				// �@��I����
				m_digDirection = DigDirection::NotDigging;
				m_digAnimationState = Tile::DigAnimationState::NotDigging;
				// �@�����ꏊ���󔒂ɐݒ�
				pStage->SetTileType(m_tilePosition.x - 1, m_tilePosition.y + 1, Tile::TileType::Empty);
				// �@�����ꏊ�̃����K�̕����ݒ�
				pStage->SetFillBrick(m_tilePosition.x - 1, m_tilePosition.y + 1);
			}
			else
			{
				// ���̌@��A�j���[�V������
				SetDigAnimationState();
			}
		}
		else
		{
			// �@�鎖���L�����Z��
			m_digDirection = DigDirection::NotDigging;
			m_digAnimationState = Tile::DigAnimationState::NotDigging;
			pStage->SetTileDigAnimationState(m_tilePosition.x - 1, m_tilePosition.y + 1, Tile::DigAnimationState::NotDigging);
		}
		return;
	}

	// X�L�[�������ꂽ���@���Ă��Ȃ��Ȃ�E�����@��
	if ((keyTrigger & PAD_INPUT_2) && (m_digDirection == DigDirection::NotDigging))
	{
		// �E�����Ɍ@��
		m_digDirection = DigDirection::Right;
		// �@��A�j���[�V�����ݒ�
		m_digAnimationState = Tile::DigAnimationState::Dig01;
	}

	// �E�����Ɍ@���Ă���Œ�
	if (m_digDirection == DigDirection::Right)
	{
		// �E�����@�邱�Ƃ��\���H
		if (IsDiggableRight(pStage))
		{
			// ��ɍ��킹��悤��X���W�𒲐�����
			AjustCloumn();
			// �s�ɍ��킹��悤��Y���W�𒲐�����
			AjustRow();

			// �v���C���[�̃A�j���[�V������ݒ�i�@���Ă���r���ŊG��ς��Ă���j
			if (m_digAnimationState == Tile::DigAnimationState::Dig01) m_playerAnimationState = PlayerAnimationState::Dig_R;
			if (m_digAnimationState == Tile::DigAnimationState::Dig06) m_playerAnimationState = PlayerAnimationState::Run01_R;

			// �X�e�[�W���̃u���b�N�̌@��A�j���[�V�����̍X�V
			pStage->SetTileDigAnimationState(m_tilePosition.x + 1, m_tilePosition.y + 1, m_digAnimationState);

			// �@��I������H
			if (m_digAnimationState == Tile::DigAnimationState::NotDigging)
			{
				// �@��I����
				m_digDirection = DigDirection::NotDigging;
				m_digAnimationState = Tile::DigAnimationState::NotDigging;
				// �@�����ꏊ���󔒂ɐݒ�
				pStage->SetTileType(m_tilePosition.x + 1, m_tilePosition.y + 1, Tile::TileType::Empty);
				// �@�����ꏊ�̃����K�̕����ݒ�
				pStage->SetFillBrick(m_tilePosition.x + 1, m_tilePosition.y + 1);
			}
			else
			{
				// ���̌@��A�j���[�V������
				SetDigAnimationState();
			}
		}
		else
		{
			// �@�鎖���L�����Z��
			m_digDirection = DigDirection::NotDigging;
			m_digAnimationState = Tile::DigAnimationState::NotDigging;
			pStage->SetTileDigAnimationState(m_tilePosition.x - 1, m_tilePosition.y + 1, Tile::DigAnimationState::NotDigging);
		}
		return;
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
				SetPlayerAnimationState(PlayerAnimationState::Rope01_L, PlayerAnimationState::Rope03_L);
			}
			else
			{
				// ����A�j���[�V����
				SetPlayerAnimationState(PlayerAnimationState::Run01_L, PlayerAnimationState::Run03_L);
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
				SetPlayerAnimationState(PlayerAnimationState::Rope01_R, PlayerAnimationState::Rope03_R);
			}
			else
			{
				// ����A�j���[�V����
				SetPlayerAnimationState(PlayerAnimationState::Run01_R, PlayerAnimationState::Run03_R);
			}
		}
	}
}

// �`�揈��
void Player::Render(int ghTileset) const
{
	// �\��(OFF)
	if (!m_isDisplay) return;

	// �v���C���[�̕`��
	POINT pos = PLAYER_SPRITES[static_cast<int>(m_playerAnimationState)];
	DrawRectGraph( m_tilePosition.x * Tile::TILE_PIXEL_WIDTH + (m_ajustPosition.x - Tile::TILE_CENTER_X) * 2
				 , m_tilePosition.y * Tile::TILE_PIXEL_HEIGHT + (m_ajustPosition.y - Tile::TILE_CENTER_Y) * 2
				 , Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				 , Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);

	// �@���Ă��鎞�̔j�Ђ̕`��
	if (m_digAnimationState != Tile::DigAnimationState::NotDigging)
	{
		const POINT* SPRITES = nullptr;
		int x = 0;

		// �@���Ă�������ɉ����ĊG��ς���
		if (m_digDirection == DigDirection::Left)
		{
			x = -1;
			SPRITES = DIG_DEBRIS_LEFT_SPRITES;
		}
		else
		{
			x = 1;
			SPRITES = DIG_DEBRIS_RIGHT_SPRITES;
		}

		POINT pos = SPRITES[static_cast<int>(m_digAnimationState)];
		// �\������j�Ђ�����Ȃ�
		if (pos.x >= 0)
		{
			// �@���Ă��鎞�̔j�Ђ̕`��
			DrawRectGraph((m_tilePosition.x + x) * Tile::TILE_PIXEL_WIDTH, m_tilePosition.y * Tile::TILE_PIXEL_HEIGHT
				, Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		}
	}


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

// ���Ɍ@��邩���ׂ�֐�
bool Player::IsDiggableLeft(Stage* pStage) const
{
	// ��ԉ��̍s�Ȃ�@��Ȃ�
	if (m_tilePosition.y == Stage::STAGE_HEIGHT - 1) return false;

	// ��ԍ��̗�Ȃ�@��Ȃ�
	if (m_tilePosition.x == 0) return false;

	// �����󔒂��B���n�V�S�łȂ�
	Tile::TileType tileType = pStage->GetTileType(m_tilePosition.x - 1, m_tilePosition.y);
	if ((tileType != Tile::TileType::Empty) && (tileType != Tile::TileType::InvisibleLadder)) return false;

	// �������u���b�N�łȂ�
	if (pStage->GetTileType(m_tilePosition.x - 1, m_tilePosition.y + 1) != Tile::TileType::Blick) return false;

	return true;
}

// �E�Ɍ@��邩���ׂ�֐�
bool Player::IsDiggableRight(Stage* pStage) const
{
	// ��ԉ��̍s�Ȃ�@��Ȃ�
	if (m_tilePosition.y == Stage::STAGE_HEIGHT - 1) return false;

	// ��ԉE�̗�Ȃ�@��Ȃ�
	if (m_tilePosition.x == Stage::STAGE_WIDTH - 1) return false;

	// �E���󔒂��B���n�V�S�łȂ�
	Tile::TileType tileType = pStage->GetTileType(m_tilePosition.x + 1, m_tilePosition.y);
	if ((tileType != Tile::TileType::Empty) && (tileType != Tile::TileType::InvisibleLadder)) return false;

	// �E�����u���b�N�łȂ�
	if (pStage->GetTileType(m_tilePosition.x + 1, m_tilePosition.y + 1) != Tile::TileType::Blick) return false;

	return true;

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

// �v���C���[�A�j���[�V�����X�e�[�g�̐ݒ�
void Player::SetPlayerAnimationState(PlayerAnimationState beginAnimState, PlayerAnimationState endAnimState)
{
	int begin = static_cast<int>(beginAnimState);
	int end = static_cast<int>(endAnimState);
	int animState = static_cast<int>(m_playerAnimationState);

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

	m_playerAnimationState = static_cast<PlayerAnimationState>(animState);
}

// �@��A�j���[�V�����X�e�[�g�̐ݒ�
void Player::SetDigAnimationState()
{
	if (static_cast<int>(m_digAnimationState) == static_cast<int>(Tile::DigAnimationState::Dig12))
	{
		m_digAnimationState = Tile::DigAnimationState::NotDigging;
	}
	else
	{
		m_digAnimationState = static_cast<Tile::DigAnimationState>(static_cast<int>(m_digAnimationState) + 1);
	}
}

