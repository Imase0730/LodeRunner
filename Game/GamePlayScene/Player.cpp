//--------------------------------------------------------------------------------------
// File: Player.cpp
//
//--------------------------------------------------------------------------------------
#include "Player.h"
#include "GamePlayScene.h"
#include "Game/Level.h"

// �R���X�g���N�^
Player::Player(GamePlayScene* pScene, Level* pLevel)
	: m_pGamePlayScene{ pScene }
	, m_pLevel{ pLevel }
	, m_isActive{ false }
	, m_isAlive{ false }
	, m_isVisible{ false }
	, m_tilePosition{ 0, 0 }
	, m_adjustPosition{ 0, 0 }
	, m_faceDirection{ Direction::Right }
	, m_playerAnimationState{ PlayerAnimationState::Run01_R }
	, m_digDirection{ DigDirection::NotDigging }
	, m_digAnimationState{ DigAnimationState::NotDigging }
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
	m_isVisible = true;

	// �ʒu������������
	m_tilePosition = tilePosition;
	m_adjustPosition = ajustPosition;

	// Page1�Ƀv���C���[�̈ʒu��ݒ肷��
	m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Player);

	// �e�ϐ��̏�����
	m_faceDirection = Direction::Right;
	m_playerAnimationState = PlayerAnimationState::Run01_R;
	m_digDirection = DigDirection::NotDigging;
	m_digAnimationState = DigAnimationState::NotDigging;
}

// �X�V����
void Player::Update(int keyCondition, int keyTrigger)
{
	// ����ł���Ȃ牽�����Ȃ�
	if (!m_isAlive) return;

	// �������Ɍ@���Ă���
	if (m_digDirection == DigDirection::Left)
	{
		// �A�j���[�V�������X�V����
		UpdateDigAnimation();
		// ���Ɍ@��
		DigLeft();
		return;
	}

	// �E�����Ɍ@���Ă���
	if (m_digDirection == DigDirection::Right)
	{
		// �A�j���[�V�������X�V����
		UpdateDigAnimation();
		// �E�Ɍ@��
		DigRight();
		return;
	}

	// ��������
	if (IsFalling())
	{
		// ������
		Falling();
		return;
	}

	// ��L�[�������ꂽ
	if (keyCondition & PAD_INPUT_UP)
	{
		// ��Ɉړ�
		MoveUp();
		return;
	}

	// ���L�[�������ꂽ
	if (keyCondition & PAD_INPUT_DOWN)
	{
		// ���Ɉړ�
		MoveDown();
		return;
	}

	// �@���Ă��Ȃ��Ȃ�
	if (m_digDirection == DigDirection::NotDigging)
	{
		// Z�L�[�������ꂽ�������@��
		if (keyTrigger & PAD_INPUT_1)
		{
			// �������Ɍ@��
			m_digDirection = DigDirection::Left;
			// �@��A�j���[�V�����ݒ�
			m_digAnimationState = DigAnimationState::Dig01;
			// ���Ɍ@��
			DigLeft();
			return;
		}

		// X�L�[�������ꂽ�E�����@��
		if (keyTrigger & PAD_INPUT_2)
		{
			// �E�����Ɍ@��
			m_digDirection = DigDirection::Right;
			// �@��A�j���[�V�����ݒ�
			m_digAnimationState = DigAnimationState::Dig01;
			// �E�Ɍ@��
			DigRight();
			return;
		}
	}

	if ((keyTrigger & PAD_INPUT_1) && (m_digDirection == DigDirection::NotDigging))
	{
		// �������Ɍ@��
		m_digDirection = DigDirection::Left;
		// �@��A�j���[�V�����ݒ�
		m_digAnimationState = DigAnimationState::Dig01;
		// ���Ɍ@��
		DigLeft();
		return;
	}

	// X�L�[�������ꂽ���@���Ă��Ȃ��Ȃ�E�����@��
	if ((keyTrigger & PAD_INPUT_2) && (m_digDirection == DigDirection::NotDigging))
	{
		// �E�����Ɍ@��
		m_digDirection = DigDirection::Right;
		// �@��A�j���[�V�����ݒ�
		m_digAnimationState = DigAnimationState::Dig01;
		// �E�Ɍ@��
		DigRight();
		return;
	}

	// ���L�[�������ꂽ
	if (keyCondition & PAD_INPUT_LEFT)
	{
		// ���Ɉړ�
		MoveLeft();
		return;
	}

	// �E�L�[�������ꂽ
	if (keyCondition & PAD_INPUT_RIGHT)
	{
		// �E�Ɉړ�
		MoveRight();
		return;
	}
}

// �`�揈��
void Player::Render(int ghTileset) const
{
	// �\��(ON)
	if (m_isVisible)
	{
		// �v���C���[�̕`��
		POINT pos = PLAYER_SPRITES[static_cast<int>(m_playerAnimationState)];
		DrawRectGraph(m_tilePosition.x * Tile::TILE_PIXEL_WIDTH + (m_adjustPosition.x - Tile::TILE_CENTER_X) * 2
			, m_tilePosition.y * Tile::TILE_PIXEL_HEIGHT + (m_adjustPosition.y - Tile::TILE_CENTER_Y) * 2
			, Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
	}

	// �@���Ă��鎞�̔j�Ђ̕`��
	if (m_digAnimationState != DigAnimationState::NotDigging)
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

		// �@���Ă���Œ��̃����K�Ȃ�
		if (m_digAnimationState != DigAnimationState::NotDigging)
		{
			// �@���Ă���Œ��̃����K�̕`��
			POINT pos = DIG_BRICK_SPRITES[static_cast<int>(m_digAnimationState)];

			// �^�C���̕`��
			DrawRectGraph((m_tilePosition.x + x) * Tile::TILE_PIXEL_WIDTH, (m_tilePosition.y + 1) * Tile::TILE_PIXEL_HEIGHT
				, Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, FALSE);
		}

	}
}

// �ʒu���ɒ�������֐�
void Player::AjustCloumn()
{
	// ��������K�v�͂Ȃ�
	if (m_adjustPosition.x == Tile::TILE_CENTER_X) return;

	// ���E�ɂ���Ă���ꍇ�͒�������
	if (m_adjustPosition.x < Tile::TILE_CENTER_X)
	{
		m_adjustPosition.x++;
	}
	else
	{
		m_adjustPosition.x--;
	}
	// ���򂪏E���邩�`�F�b�N
	CheckGoldPickedUp();
}

// �ʒu���s�ɒ�������֐�
void Player::AjustRow()
{
	// ��������K�v�͂Ȃ�
	if (m_adjustPosition.y == Tile::TILE_CENTER_Y) return;

	// �㉺�ɂ���Ă���ꍇ�͒�������
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y)
	{
		m_adjustPosition.y++;
	}
	else
	{
		m_adjustPosition.y--;
	}
	// ���򂪏E���邩�`�F�b�N
	CheckGoldPickedUp();
}

// �������Ă��邩�`�F�b�N����֐�
bool Player::IsFalling()
{
	// �n�V�S�Ȃ�ړ��\
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Ladder) return false;

	// ���[�v�ɂ���ł���Έړ��\
	if ( (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Rope)
	  && (m_adjustPosition.y == Tile::TILE_CENTER_Y)
	   )
	{
		return false;
	}

	// �󒆂ɕ����Ă���
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y) return true;

	// ��ԉ��Ȃ�ړ��\
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// ���̍s�������\�Ȃ�
	Tile::Type page1 = m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y + 1);
	Tile::Type page2 = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y + 1);
	if (IsMovableTileFall(page1, page2)) return true;

	return false;
}

// ��Ɉړ��\�����ׂ�֐�
bool Player::IsMovableUp()
{
	// �n�V�S������H
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Ladder)
	{
		// �n�V�S��Ȃ̂Ńn�V�S�̐^�񒆂܂ł͈ړ��\
		if (m_adjustPosition.y > Tile::TILE_CENTER_Y) return true;

		// ��ԏ�Ȃ�ړ��s��
		if (m_tilePosition.y == 0) return false;

		// ��̍s���ړ��\�Ȃ�
		if (IsMovableTileULR(m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y - 1))) return true;
	}
	else
	{
		if (m_adjustPosition.y > Tile::TILE_CENTER_Y) return true;
	}
	return false;
}

bool Player::IsMovableDown()
{
	// ������ɂ���̂ŉ��Ɉړ��\
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y) return true;

	// ��ԉ��Ȃ�ړ��s��
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// ���̍s���ړ��\�Ȃ�
	if (IsMovableTileDown(m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y + 1))) return true;

	return false;
}

// ���Ɍ@��邩���ׂ�֐�
bool Player::IsDiggableLeft()
{
	// ��ԉ��̍s�Ȃ�@��Ȃ�
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// ��ԍ��̗�Ȃ�@��Ȃ�
	if (m_tilePosition.x == 0) return false;

	// �������u���b�N�łȂ�
	if (m_pLevel->GetTilePage1(m_tilePosition.x - 1, m_tilePosition.y + 1) != Tile::Type::Blick) return false;

	// ������
	if (m_pLevel->GetTilePage1(m_tilePosition.x - 1, m_tilePosition.y) != Tile::Type::Empty) return false;

	return true;
}

// �E�Ɍ@��邩���ׂ�֐�
bool Player::IsDiggableRight()
{
	// ��ԉ��̍s�Ȃ�@��Ȃ�
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// ��ԉE�̗�Ȃ�@��Ȃ�
	if (m_tilePosition.x == Level::MAX_GAME_COLMUN) return false;

	// �E�����u���b�N�łȂ�
	if (m_pLevel->GetTilePage1(m_tilePosition.x + 1, m_tilePosition.y + 1) != Tile::Type::Blick) return false;

	// �E����
	if (m_pLevel->GetTilePage1(m_tilePosition.x + 1, m_tilePosition.y) != Tile::Type::Empty) return false;

	return true;
}

// ���Ɉړ��\�����ׂ�֐�
bool Player::IsMovableLeft()
{
	// �܂��������Ɉړ�������]�n������
	if (m_adjustPosition.x > Tile::TILE_CENTER_X) return true;

	// �����Ȃ̂ňړ��s��
	if (m_tilePosition.x == 0) return false;

	// �ړ���^�C�����ړ��\�����ׂ�
	return IsMovableTileULR(m_pLevel->GetTilePage1(m_tilePosition.x - 1, m_tilePosition.y));
}

// �E�Ɉړ��\�����ׂ�֐�
bool Player::IsMovableRight()
{
	// �܂��E�����Ɉړ�������]�n������
	if (m_adjustPosition.x < Tile::TILE_CENTER_X) return true;

	// �E���Ȃ̂ňړ��s��
	if (m_tilePosition.x == Level::MAX_GAME_COLMUN) return false;

	// �ړ���^�C�����ړ��\�����ׂ�
	return IsMovableTileULR(m_pLevel->GetTilePage1(m_tilePosition.x + 1, m_tilePosition.y));
}

// ���Ɍ@��
void Player::DigLeft()
{
	// �������@�邱�Ƃ��\���H
	if (IsDiggableLeft())
	{
		// ��ɍ��킹��悤��X���W�𒲐�����
		AjustCloumn();
		// �s�ɍ��킹��悤��Y���W�𒲐�����
		AjustRow();

		// �v���C���[�̃A�j���[�V������ݒ�i�@���Ă���r���ŊG��ς��Ă���j
		if (m_digAnimationState == DigAnimationState::Dig01) m_playerAnimationState = PlayerAnimationState::Dig_L;
		if (m_digAnimationState == DigAnimationState::Dig06) m_playerAnimationState = PlayerAnimationState::Run01_L;

		// �@��I������H
		if (m_digAnimationState == DigAnimationState::NotDigging)
		{
			// �@��I����
			m_digDirection = DigDirection::NotDigging;
			// �@�����ꏊ���󔒂ɐݒ�
			m_pLevel->SetTilePage1(m_tilePosition.x - 1, m_tilePosition.y + 1, Tile::Type::Empty);
			// �@�����ꏊ�̃����K�̕����ݒ�
			m_pGamePlayScene->SetFillBrick(m_tilePosition.x - 1, m_tilePosition.y + 1);
		}
	}
	else
	{
		// �@�鎖���L�����Z��
		m_digDirection = DigDirection::NotDigging;
		m_digAnimationState = DigAnimationState::NotDigging;
	}
}

// �E�Ɍ@��
void Player::DigRight()
{
	// �E�����@�邱�Ƃ��\���H
	if (IsDiggableRight())
	{
		// ��ɍ��킹��悤��X���W�𒲐�����
		AjustCloumn();
		// �s�ɍ��킹��悤��Y���W�𒲐�����
		AjustRow();

		// �v���C���[�̃A�j���[�V������ݒ�i�@���Ă���r���ŊG��ς��Ă���j
		if (m_digAnimationState == DigAnimationState::Dig01) m_playerAnimationState = PlayerAnimationState::Dig_R;
		if (m_digAnimationState == DigAnimationState::Dig06) m_playerAnimationState = PlayerAnimationState::Run01_R;

		// �@��I������H
		if (m_digAnimationState == DigAnimationState::NotDigging)
		{
			// �@��I����
			m_digDirection = DigDirection::NotDigging;
			// �@�����ꏊ���󔒂ɐݒ�
			m_pLevel->SetTilePage1(m_tilePosition.x + 1, m_tilePosition.y + 1, Tile::Type::Empty);
			// �@�����ꏊ�̃����K�̕����ݒ�
			m_pGamePlayScene->SetFillBrick(m_tilePosition.x + 1, m_tilePosition.y + 1);
		}
	}
	else
	{
		// �@�鎖���L�����Z��
		m_digDirection = DigDirection::NotDigging;
		m_digAnimationState = DigAnimationState::NotDigging;
	}
}

// ������
void Player::Falling()
{
	// ��ɍ��킹��悤��X���W�𒲐�����
	AjustCloumn();

	// ����
	m_adjustPosition.y++;
	if (m_adjustPosition.y > 4)
	{
		m_adjustPosition.y = 0;
		// Page1�����ɖ߂�
		m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
		// Page1�ɏ�������
		m_tilePosition.y++;
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Player);
	}

	// ���򂪏E���邩�`�F�b�N
	CheckGoldPickedUp();

	// �����A�j���[�V����
	if (m_faceDirection == Direction::Left)
	{
		m_playerAnimationState = PlayerAnimationState::Fall_L;
	}
	else
	{
		m_playerAnimationState = PlayerAnimationState::Fall_R;
	}
}

// ��Ɉړ�
void Player::MoveUp()
{
	// ��Ɉړ��\���H
	if (IsMovableUp())
	{
		// ��ɍ��킹��悤��X���W�𒲐�����
		AjustCloumn();

		// ��Ɉړ�����
		m_adjustPosition.y--;
		if (m_adjustPosition.y < 0)
		{
			m_adjustPosition.y = 4;
			// Page1�����ɖ߂�
			m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
			// Page1�ɏ�������
			m_tilePosition.y--;
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Player);
		}

		// ���򂪏E���邩�`�F�b�N
		CheckGoldPickedUp();

		// �n�V�S��o��A�j���[�V����
		SetPlayerAnimationState(PlayerAnimationState::Climb01, PlayerAnimationState::Climb02);
	}
}

// ���Ɉړ�
void Player::MoveDown()
{
	// ���Ɉړ��\���H
	if (IsMovableDown())
	{
		// ��ɍ��킹��悤��X���W�𒲐�����
		AjustCloumn();

		// ���Ɉړ�����
		m_adjustPosition.y++;
		if (m_adjustPosition.y > 4)
		{
			m_adjustPosition.y = 0;
			// Page1�����ɖ߂�
			m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
			// Page1�ɏ�������
			m_tilePosition.y++;
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Player);
		}

		// ���򂪏E���邩�`�F�b�N
		CheckGoldPickedUp();

		// �n�V�S��o��A�j���[�V����
		SetPlayerAnimationState(PlayerAnimationState::Climb01, PlayerAnimationState::Climb02);
	}
}

// ���Ɉړ�
void Player::MoveLeft()
{
	// ���Ɉړ��\���H
	if (IsMovableLeft())
	{
		// �s�ɍ��킹��悤��Y���W�𒲐�����
		AjustRow();

		// ��������
		m_faceDirection = Direction::Left;

		// ���Ɉړ�����
		m_adjustPosition.x--;
		if (m_adjustPosition.x < 0)
		{
			m_adjustPosition.x = 4;
			// Page1�����ɖ߂�
			m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
			// Page1�ɏ�������
			m_tilePosition.x--;
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Player);
		}

		// ���򂪏E���邩�`�F�b�N
		CheckGoldPickedUp();

		// ���[�v�̏ꍇ
		if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Rope)
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

// �E�Ɉړ�
void Player::MoveRight()
{
	// �E�Ɉړ��\���H
	if (IsMovableRight())
	{
		// �s�ɍ��킹��悤��Y���W�𒲐�����
		AjustRow();

		// �E������
		m_faceDirection = Direction::Right;

		// �E�ړ�����
		m_adjustPosition.x++;
		if (m_adjustPosition.x > 4)
		{
			m_adjustPosition.x = 0;
			// Page1�����ɖ߂�
			m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
			// Page1�ɏ�������
			m_tilePosition.x++;
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Player);
		}

		// ���򂪏E���邩�`�F�b�N
		CheckGoldPickedUp();

		// ���[�v�̏ꍇ
		if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Rope)
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
void Player::UpdateDigAnimation()
{
	if (static_cast<int>(m_digAnimationState) == static_cast<int>(DigAnimationState::Dig12))
	{
		m_digAnimationState = DigAnimationState::NotDigging;
	}
	else
	{
		m_digAnimationState = static_cast<DigAnimationState>(static_cast<int>(m_digAnimationState) + 1);
	}
}

// ���򂪏E���邩���ׂ�֐�
void Player::CheckGoldPickedUp()
{
	// �^�C���̒��S�łȂ���΃`�F�b�N���Ȃ�
	if (m_adjustPosition.x != Tile::TILE_CENTER_X) return;
	if (m_adjustPosition.y != Tile::TILE_CENTER_Y) return;

	// ���򂪂���΋�����E��
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Gold)
	{
		// �󔒂ɂ���
		m_pLevel->SetTilePage2(m_tilePosition.x, m_tilePosition.y, Tile::Type::Empty);
		// �X�e�[�W��̋���̐������炷
		m_pLevel->LostGold();
		// ���_�����Z�i�Q�T�O�_�j
		m_pGamePlayScene->AddScore(GamePlayScene::GOLD_SCORE);
	}
}

bool Player::IsMovableTileFall(Tile::Type page1, Tile::Type page2)
{
	// �󔒂Ȃ痎��
	if (page1 == Tile::Type::Empty)
	{
		return true;
	}

	// �K�[�h�Ȃ痎�����Ȃ�
	if (page1 == Tile::Type::Guard)
	{
		return false;
	}

	// �����K�A�΁A�n�V�S�Ȃ痎�����Ȃ�
	if ((page2 == Tile::Type::Blick) || (page2 == Tile::Type::Stone) || (page2 == Tile::Type::Ladder))
	{
		return false;
	}

	return true;
}

// �ړ��\�ȃ^�C�������ׂ�֐��i�㍶�E�j
bool Player::IsMovableTileULR(Tile::Type tile)
{
	if ( (tile == Tile::Type::Blick)	// page1:�����K
	  || (tile == Tile::Type::Stone)	// page1:��
	  || (tile == Tile::Type::Trap)	// page1:�
	   )
	{
		// �ړ��s��
		return false;
	}
	// �ړ��\
	return true;
}

// �ړ��\�ȃ^�C�������ׂ�֐��i���j
bool Player::IsMovableTileDown(Tile::Type tile)
{
	if ( (tile == Tile::Type::Blick)	// page1:�����K
	  || (tile == Tile::Type::Stone)	// page1:��
	   )
	{
		// �ړ��s��
		return false;
	}
	// �ړ��\
	return true;
}

