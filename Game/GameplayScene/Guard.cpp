//--------------------------------------------------------------------------------------
// File: Guard.cpp
//
//--------------------------------------------------------------------------------------
#include "Guard.h"
#include "GamePlayScene.h"
#include "Game/Level.h"
#include "Player.h"

// �R���X�g���N�^
Guard::Guard(GamePlayScene* pScene, Level* pLevel)
	: m_pGamePlayScene{ pScene }
	, m_pLevel{ pLevel }
	, m_isActive{ false }
	, m_isAlive{ false }
	, m_isDisplay{ false }
	, m_tilePosition{ 0, 0 }
	, m_adjustPosition{ 0, 0 }
	, m_faceDirection{ Direction::Left }
	, m_guardAnimationState{ GuardAnimationState::Run01_L }
	, m_actionStateTimer{ 0 }
	, m_resurrectionTimer{ 0 }
{
}

// �f�X�g���N�^
Guard::~Guard()
{
}

// ����������
void Guard::Initialize(POINT tilePosition, POINT ajustPosition)
{
	// �A�N�e�B�u�ɂ���
	m_isActive = true;

	// �����t���O������
	m_isAlive = true;

	// �\��(ON)
	m_isDisplay = true;

	// �ʒu������������
	m_tilePosition = tilePosition;
	m_adjustPosition = ajustPosition;

	// Page1�ɃK�[�h�̈ʒu��ݒ肷��
	m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Guard);

	// �e�ϐ��̏�����
	m_faceDirection = Direction::Left;
	m_guardAnimationState = GuardAnimationState::Run01_L;
	m_actionStateTimer = 0;
	m_resurrectionTimer = 0;
}

// �X�V����
void Guard::Update()
{
	// ���ɗ��������̏���
	if (HandleFallingInHole()) return;

	// ��������
	if (HandlFalling()) return;

	// �ړ�����
	HandleMovement();
}

// ���ɗ��������̏���
bool Guard::HandleFallingInHole()
{
	// ���̒��H
	if (IsInHole())
	{
		// ���ɗ����Ă���^�C�}�[�����Z
		m_actionStateTimer--;

		// ���̒��Ȃ痎���Ȃ��悤�Ƀ^�C�}�[���O�ɂ��Ȃ��悤�ɂ���
		if ( (m_actionStateTimer == 0)
		  && (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Blick)
		   )
		{
			m_actionStateTimer++;
		}

		// ���ɗ���������A���t���[���̓A�j���[�V�������Ȃ�
		if (m_actionStateTimer > HOLE_ANIMATION_START_FRAME) return true;

		// ���ɗh���A�j���[�V����
		int idx = m_actionStateTimer - (HOLE_ANIMATION_START_FRAME - HOLE_ANIMATION_TABLE_SIZE + 1);
		if (idx >= 0)
		{
			m_adjustPosition.x = HOLE_ANIMATION_TABLE[idx];
			return true;
		}
	}
	return false;
}

// ��������
bool Guard::HandlFalling()
{
	// ���ɗ����Ă���Œ��͗��������A�����オ�邽�ߗ����Ȃ��悤�ɂ���
	if (!IsInHole())
	{
		// ��������
		if (IsFalling())
		{
			// ������
			Falling();
			return true;
		}
	}
	return false;
}

// �ړ�����
void Guard::HandleMovement()
{
	// �ړ����������߂�
	switch (DecideMoveDirection())
	{
	case Guard::MoveDirection::UP:
		MoveUp();
		break;
	case Guard::MoveDirection::Down:
		MoveDown();
		break;
	case Guard::MoveDirection::Left:
		MoveLeft();
		break;
	case Guard::MoveDirection::Right:
		MoveRight();
		break;
	default:
		break;
	}
}

// ���̒������ׂ�֐�
bool Guard::IsInHole() const
{
	if (m_actionStateTimer > 0) return true;
	return false;
}

// �`�揈��
void Guard::Render(int ghTileset)
{
	// �\��(OFF)
	if (!m_isDisplay) return;

	// ������
	if (m_resurrectionTimer > 0)
	{
		// �����̃A�j���[�V����
		if (m_resurrectionTimer == RESURRECTION_ANIME_TIME_EGG01) m_guardAnimationState = GuardAnimationState::Egg01;
		if (m_resurrectionTimer == RESURRECTION_ANIME_TIME_EGG02) m_guardAnimationState = GuardAnimationState::Egg02;
	}

	// �K�[�h�̕`��
	POINT pos = GUARD_SPRITES[static_cast<int>(m_guardAnimationState)];
	DrawRectGraph( m_tilePosition.x * Tile::TILE_PIXEL_WIDTH + (m_adjustPosition.x - Tile::TILE_CENTER_X) * 2
				 , m_tilePosition.y * Tile::TILE_PIXEL_HEIGHT + (m_adjustPosition.y - Tile::TILE_CENTER_Y) * 2
				 , Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				 , Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
}

// �w��ʒu���畜������֐�
void Guard::Resurrection(int colmun, int row)
{
	// �A�j���[�V������������
	m_guardAnimationState = GuardAnimationState::AnimNone;
	// �ʒu��ݒ�
	SetTilePosition(colmun, row);
	// �����^�C�}�[��ݒ�
	SetResurrectionTimer(RESURRECTION_ANIME_TIME_NONE);
}

// �ʒu���ɒ�������֐�
void Guard::AjustCloumn()
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
void Guard::AjustRow()
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
bool Guard::IsFalling()
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
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y)
	{
		return true;
	}

	// ��ԉ��Ȃ�ړ��\
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// ���̍s�������\�Ȃ�
	Tile::Type page1 = m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y + 1);
	Tile::Type page2 = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y + 1);
	if (IsMovableTileFall(page1, page2)) return true;

	return false;
}

// ��Ɉړ��\�����ׂ�֐�
bool Guard::IsMovableUp()
{
	// �n�V�S��Ȃ̂Ńn�V�S�̐^�񒆂܂ł͈ړ��\
	if (m_adjustPosition.y > Tile::TILE_CENTER_Y) return true;

	// ��ԏ�Ȃ�ړ��s��
	if (m_tilePosition.y == 0) return false;

	// ��̍s���ړ��\�Ȃ�
	Tile::Type page1 = m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y - 1);
	if (IsMovableTileULR(page1)) return true;

	return false;
}

bool Guard::IsMovableDown()
{
	// ������ɂ���̂ŉ��Ɉړ��\
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y) return true;

	// ��ԉ��Ȃ�ړ��s��
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// ���̍s���ړ��\�Ȃ�
	Tile::Type page1 = m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y + 1);
	if (IsMovableTileD(page1)) return true;

	return false;
}

// ���Ɉړ��\�����ׂ�֐�
bool Guard::IsMovableLeft()
{
	// �܂��������Ɉړ�������]�n������
	if (m_adjustPosition.x > Tile::TILE_CENTER_X) return true;

	// �����Ȃ̂ňړ��s��
	if (m_tilePosition.x == 0) return false;

	// �ړ���^�C�����ړ��\�����ׂ�
	Tile::Type page1 = m_pLevel->GetTilePage1(m_tilePosition.x - 1, m_tilePosition.y);
	if (IsMovableTileULR(page1)) return true;

	return false;
}

// �E�Ɉړ��\�����ׂ�֐�
bool Guard::IsMovableRight()
{
	// �܂��E�����Ɉړ�������]�n������
	if (m_adjustPosition.x < Tile::TILE_CENTER_X) return true;

	// �E���Ȃ̂ňړ��s��
	if (m_tilePosition.x == Level::MAX_GAME_COLMUN) return false;

	// �ړ���^�C�����ړ��\�����ׂ�
	Tile::Type page1 = m_pLevel->GetTilePage1(m_tilePosition.x + 1, m_tilePosition.y);
	if (IsMovableTileULR(page1)) return true;

	return false;
}

// ������
void Guard::Falling()
{
	// ��ɍ��킹��悤��X���W�𒲐�����
	AjustCloumn();

	// ����
	m_adjustPosition.y++;
	if (m_adjustPosition.y > 4)
	{
		// ���̃^�C���ֈړ������̂�Page1�̃^�C����������������
		m_adjustPosition.y = 0;

		Tile::Type page2 = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y);
		if (page2 != Tile::Type::Blick)
		{
			// Page2��Page1�փR�s�[
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, page2);
		}
		else
		{
			// �@��������ʉ�
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Empty);
		}

		// ���̍s�ֈړ�
		m_tilePosition.y++;

		// �v���C���[�Ȃ玀�S
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}

		// �@�������Ȃ�
		if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Blick)
		{
			// ����������Ă���
			if (m_actionStateTimer < 0)
			{
				// ����𗎂Ƃ�
				m_actionStateTimer = 0;
				// ���򂪒u���邩�H
				if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y - 1) == Tile::Type::Empty)
				{
					// �������ɒu��
					m_pLevel->SetTilePage2(m_tilePosition.x, m_tilePosition.y - 1, Tile::Type::Gold);
				}
				else
				{
					// ��������X�g����
					m_pLevel->LostGold();
				}
			}
		}

		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Guard);
	}
	else
	{
		// �������̃^�C���`�F�b�N������
		if (m_adjustPosition.y == Tile::TILE_CENTER_Y)
		{
			// ���򂪏E���邩�`�F�b�N
			CheckGoldPickedUp();

			// �@�������Ȃ�
			if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Blick)
			{
				// ����������Ă���΃��X�g����
				if (m_actionStateTimer < 0) m_pLevel->LostGold();

				// �s���^�C�}�[�ݒ�i�����p�Ɏg�p����j
				m_actionStateTimer = RESURRECTION_TIME;

				// ���_�����Z�i�V�T�_�j
				m_pGamePlayScene->AddScore(GamePlayScene::GUARD_HOLE_SCORE);

				// ���ʉ���炷�i���ɗ����鉹�j
				m_pGamePlayScene->GetSound().PlaySound(Sound::SoundID::InHole);
			}
		}
	}

	// �����A�j���[�V����
	if (m_faceDirection == Direction::Left)
	{
		m_guardAnimationState = GuardAnimationState::Fall_L;
	}
	else
	{
		m_guardAnimationState = GuardAnimationState::Fall_R;
	}
}

// �ړ����������߂�֐�
Guard::MoveDirection Guard::DecideMoveDirection()
{
	// ���̒�
	if ( (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Blick)
	  && (m_actionStateTimer > 0)	// ���ɗ����Ă���
	   )
	{
		// ���
		return MoveDirection::UP;
	}

	// �v���C���[�̈ʒu���擾
	Player* pPlayer = m_pGamePlayScene->GetPlayer();
	int playerColumn = pPlayer->GetTilePosition().x;
	int playerRow = pPlayer->GetTilePosition().y;

	// �v���C���[�Ɠ����s�ɂ���
	if (m_tilePosition.y == playerRow)
	{
		// �v���C���[�̈ʒu�܂ňړ��\���H
		if (IsMovableColumn(playerColumn))
		{
			// ���E�ǂ���Ƀv���C���[�����邩�`�F�b�N����
			if (m_tilePosition.x > playerColumn)
			{
				// ���Ɉړ�
				return MoveDirection::Left;
			}
			else
			{
				// �E�Ɉړ�
				return MoveDirection::Right;
			}
		}
	}

	// ���E�̈ʒu���ׂ�ʒu���擾
	int leftColmunLimit = 0;
	int rightColmunLimit = 0;
	GetLeftRightLimits(&leftColmunLimit, Direction::Left, 0);
	GetLeftRightLimits(&rightColmunLimit, Direction::Right, Level::MAX_GAME_COLMUN);

	// �v���C���[�ƃK�[�h�Ƃ̋^���I�ȋ��������Z�b�g
	int bestGuardDistance = 255;

	// �ǂ̕����ֈړ����ׂ������߂�
	MoveDirection moveDirection = MoveDirection::None;
	MoveDirection dir{};
	dir = SelectMoveUpAndDown(&bestGuardDistance);
	if (dir != MoveDirection::None) moveDirection = dir;
	dir = SelectMoveLeftAndRight(&bestGuardDistance, leftColmunLimit, Direction::Left);
	if (dir != MoveDirection::None) moveDirection = dir;
	dir = SelectMoveLeftAndRight(&bestGuardDistance, rightColmunLimit, Direction::Right);
	if (dir != MoveDirection::None) moveDirection = dir;

	return moveDirection;
}

// ��Ɉړ�
void Guard::MoveUp()
{
	// ��Ɉړ��ł��Ȃ�
	if (!IsMovableUp()) return;

	// ��ɍ��킹��悤��X���W�𒲐�����
	AjustCloumn();

	// ��Ɉړ�����
	m_adjustPosition.y--;
	if (m_adjustPosition.y < 0)
	{
		m_adjustPosition.y = 4;
		// Page1�����ɖ߂�
		m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
		// ��Ƀ^�C�����ړ�
		m_tilePosition.y--;
		// �v���C���[�Ȃ玀�S
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}
		// Page1�ɏ�������
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Guard);
	}

	// ���򂪏E���邩�`�F�b�N
	CheckGoldPickedUp();

	//�X�e�[�g���X�V���ċ���𗎂Ƃ�����
	UpdateGoldDropTimer();

	// �n�V�S��o��A�j���[�V����
	SetGuardAnimationState(GuardAnimationState::Climb01, GuardAnimationState::Climb02);
}

// ���Ɉړ�
void Guard::MoveDown()
{
	// ���Ɉړ��ł��Ȃ�
	if (!IsMovableDown()) return;

	// ��ɍ��킹��悤��X���W�𒲐�����
	AjustCloumn();

	// ���Ɉړ�����
	m_adjustPosition.y++;
	if (m_adjustPosition.y > 4)
	{
		m_adjustPosition.y = 0;
		// Page1�����ɖ߂�
		m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
		// ���Ƀ^�C�����ړ�
		m_tilePosition.y++;
		// �v���C���[�Ȃ玀�S
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}
		// Page1�ɏ�������
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Guard);
	}

	// ���򂪏E���邩�`�F�b�N
	CheckGoldPickedUp();

	//�X�e�[�g���X�V���ċ���𗎂Ƃ�����
	UpdateGoldDropTimer();

	// �n�V�S��o��A�j���[�V����
	SetGuardAnimationState(GuardAnimationState::Climb01, GuardAnimationState::Climb02);
}

// ���Ɉړ�
void Guard::MoveLeft()
{
	// ���Ɉړ��ł��Ȃ�
	if (!IsMovableLeft()) return;

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
		// ���Ƀ^�C�����ړ�
		m_tilePosition.x--;
		// �v���C���[�Ȃ玀�S
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}
		// Page1�ɏ�������
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Guard);
	}

	// ���򂪏E���邩�`�F�b�N
	CheckGoldPickedUp();

	//�X�e�[�g���X�V���ċ���𗎂Ƃ�����
	UpdateGoldDropTimer();

	// ���[�v�̏ꍇ
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Rope)
	{
		// ���[�v�ňړ�����A�j���[�V����
		SetGuardAnimationState(GuardAnimationState::Rope01_L, GuardAnimationState::Rope03_L);
	}
	else
	{
		// ����A�j���[�V����
		SetGuardAnimationState(GuardAnimationState::Run01_L, GuardAnimationState::Run03_L);
	}
}

// �E�Ɉړ�
void Guard::MoveRight()
{
	// �E�Ɉړ��ł��Ȃ�
	if (!IsMovableRight()) return;

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
		// �E�Ƀ^�C�����ړ�
		m_tilePosition.x++;
		// �v���C���[�Ȃ玀�S
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}
		// Page1�ɏ�������
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Guard);
	}

	// ���򂪏E���邩�`�F�b�N
	CheckGoldPickedUp();

	//�X�e�[�g���X�V���ċ���𗎂Ƃ�����
	UpdateGoldDropTimer();

	// ���[�v�̏ꍇ
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Rope)
	{
		// ���[�v�ňړ�����A�j���[�V����
		SetGuardAnimationState(GuardAnimationState::Rope01_R, GuardAnimationState::Rope03_R);
	}
	else
	{
		// ����A�j���[�V����
		SetGuardAnimationState(GuardAnimationState::Run01_R, GuardAnimationState::Run03_R);
	}
}

// �A�j���[�V�����X�e�[�g�̐ݒ�
void Guard::SetGuardAnimationState(GuardAnimationState beginAnimState, GuardAnimationState endAnimState)
{
	int begin = static_cast<int>(beginAnimState);
	int end = static_cast<int>(endAnimState);
	int animState = static_cast<int>(m_guardAnimationState);

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

	m_guardAnimationState = static_cast<GuardAnimationState>(animState);
}

// ���򂪏E���邩���ׂ�֐�
void Guard::CheckGoldPickedUp()
{
	// �^�C���̒��S�łȂ���΃`�F�b�N���Ȃ�
	if (m_adjustPosition.x != Tile::TILE_CENTER_X) return;
	if (m_adjustPosition.y != Tile::TILE_CENTER_Y) return;

	// ���򂪂Ȃ�
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) != Tile::Type::Gold) return;

	// ���ɋ���������Ă���
	if (m_actionStateTimer < 0) return;

	// ���򂪂���΋�����E���i����ێ��^�C�}�[��ݒ�j
	m_actionStateTimer = -0x53;

	// �󔒂ɂ���
	m_pLevel->SetTilePage2(m_tilePosition.x, m_tilePosition.y, Tile::Type::Empty);
}

// �w�肵���s�܂ňړ��\�����ׂ�֐�
bool Guard::IsMovableColumn(int column)
{
	int guardColumn = m_tilePosition.x;
	int guardRow = m_tilePosition.y;
	int move = 0;

	// ���ׂĂ���������ݒ�
	if (guardColumn > column)
	{
		// ����
		move = -1;
	}
	else
	{
		// �E��
		move = 1;
	}
	while (guardColumn != column)
	{
		// ���̍s��
		guardColumn += move;
		// �n�V�S�܂��̓��[�v�Ȃ牺�����ł����Ă��ړ��\
		if ( (m_pLevel->GetTilePage2(guardColumn, guardRow) == Tile::Type::Ladder)
		  || (m_pLevel->GetTilePage2(guardColumn, guardRow) == Tile::Type::Rope)
		   )
		{
			continue;
		}
		// �����󔒂�㩂łȂ��Ȃ�ړ��\
		if ( (guardRow == Level::MAX_GAME_ROW)
		  || ( (m_pLevel->GetTilePage2(guardColumn, guardRow + 1) != Tile::Type::Empty)
		    && (m_pLevel->GetTilePage2(guardColumn, guardRow + 1) != Tile::Type::Trap)
		     )
		   )
		{
			continue;
		}
		// �ړ��s��
		return false;
	}
	// �ړ��\
	return true;
}

// ���E�ǂ��܂ňړ��ł��邩�擾����֐�
void Guard::GetLeftRightLimits(int* colmun, Direction direction, int limit)
{
	*colmun = m_tilePosition.x;
	int row = m_tilePosition.y;

	int move = 1;
	if (direction == Direction::Left) move = -1;

	while (*colmun != limit)
	{
		// �����K�܂��͐΂Ȃ�I��
		if ( (m_pLevel->GetTilePage1(*colmun + move, row) == Tile::Type::Blick)		// �����K
		  || (m_pLevel->GetTilePage1(*colmun + move, row) == Tile::Type::Stone)		// ��
		   )
		{
			break;
		}

		// �n�V�S�܂��̓��[�v�łȂ�
		if ( (m_pLevel->GetTilePage1(*colmun + move, row) != Tile::Type::Ladder)		// �n�V�S
		  && (m_pLevel->GetTilePage1(*colmun + move, row) != Tile::Type::Rope)			// ���[�v
		   )
		{
			// �s����ԉ��łȂ�
			if (row != Level::MAX_GAME_ROW)
			{
				// ���������K�A�΁A�n�V�S�łȂ��Ȃ�I��
				if ( (m_pLevel->GetTilePage2(*colmun + move, row + 1) != Tile::Type::Blick)	// �����K
				  && (m_pLevel->GetTilePage2(*colmun + move, row + 1) != Tile::Type::Stone)	// ��
				  && (m_pLevel->GetTilePage2(*colmun + move, row + 1) != Tile::Type::Ladder)	// �n�V�S
				   )
				{
					// �ꉞ�P��܂ł͈ړ��\
					*colmun += move;
					break;
				}
			}
		}
		// ����
		*colmun += move;
	}
}

// �������Ɉړ��ȍœK�ȍs��������֐�
int Guard::FindCandidateRowBelow(int colmun, int row)
{
	// �v���C���[�̂���s
	int playerRow = m_pGamePlayScene->GetPlayer()->GetTilePosition().y;

	// ��ԉ��̍s�܂Œ��ׂ�
	while (row != Level::MAX_GAME_ROW)
	{
		// �P���̍s�������K���΂Ȃ�I��
		if ( (m_pLevel->GetTilePage2(colmun, row + 1) == Tile::Type::Blick)	// �����K
		  || (m_pLevel->GetTilePage2(colmun, row + 1) == Tile::Type::Stone)	// ��
			)
		{
			return row;
		}
		// �P�s����
		row++;
		// �󔒂łȂ�
		if (m_pLevel->GetTilePage2(colmun, row) != Tile::Type::Empty)
		{
			// �����͈ړ��\�Ȃ�
			if (CheckLeftRightMove(colmun, row, Direction::Left))
			{
				// �v���C���[�̕������̍s�Ȃ�
				if (row >= playerRow) return row;
			}
			// �E���͈ړ��\�Ȃ�
			if (CheckLeftRightMove(colmun, row, Direction::Right))
			{
				// �v���C���[�̕������̍s�Ȃ�
				if (row >= playerRow) return row;
			}
		}
	}
	return row;
}

// ������Ɉړ��ȍœK�ȍs��������֐�
int Guard::FindCandidateRowAbove(int colmun, int row)
{
	// �v���C���[�̂���s
	int playerRow = m_pGamePlayScene->GetPlayer()->GetTilePosition().y;

	// ��ԏ�̍s�܂Œ��ׂ�
	while (row != 0)
	{
		// �n�V�S�łȂ��̂ŏ�Ɉړ��ł��Ȃ��̂ŏI��
		if (m_pLevel->GetTilePage2(colmun, row) != Tile::Type::Ladder)
		{
			return row;
		}
		// �P��̍s���`�F�b�N
		row--;
		// �����͈ړ��\�Ȃ�
		if (CheckLeftRightMove(colmun, row, Direction::Left))
		{
			// �v���C���[�̕�����̍s�Ȃ�
			if (row <= playerRow)
			{
				return row;
			}
		}
		// �E���͈ړ��\�Ȃ�
		if (CheckLeftRightMove(colmun, row, Direction::Right))
		{
			// �v���C���[�̕�����̍s�Ȃ�
			if (row <= playerRow)
			{
				return row;
			}
		}
	}
	return row;
}

// ���E�Ɉړ��ł����������ׂ�֐�
bool Guard::CheckLeftRightMove(int colmun, int row, Direction direction)
{
	// �w������̗�𒲂ׂ�
	if (direction == Direction::Left)
	{
		colmun--;
	}
	else if (direction == Direction::Right)
	{
		colmun++;
	}

	// ��ʊO�Ȃ�`�F�b�N���Ȃ�
	if ((colmun < 0) || (colmun > Level::MAX_GAME_COLMUN)) return false;

	if (m_pLevel->GetTilePage2(colmun, row) == Tile::Type::Rope)	// ���[�v
	{
		return true;
	}
	else
	{
		// �����������K�A�΁A�n�V�S�Ȃ�
		if ( (m_pLevel->GetTilePage2(colmun, row + 1) == Tile::Type::Blick)	// �����K
		  || (m_pLevel->GetTilePage2(colmun, row + 1) == Tile::Type::Stone)	// ��
		  || (m_pLevel->GetTilePage2(colmun, row + 1) == Tile::Type::Ladder)	// �n�V�S
		   )
		{
			return true;
		}
	}
	return false;
}

// �㉺�ֈړ����邩���ׂ�֐�
//
// �K�[�h�̌��݂���ʒu�̏㉺�𒲂׍ł��^�������̋߂������ֈړ�����
Guard::MoveDirection Guard::SelectMoveUpAndDown(int* bestGuardDistance)
{
	MoveDirection moveDirection = MoveDirection::None;

	// �K�[�h�̈ʒu
	int colmun = m_tilePosition.x;
	int row = m_tilePosition.y;

	// ���Ɉړ��\�Ȃ�
	if (IsMovableDown(colmun, row))
	{
		// ���Ɉړ��������̍œK�ȍs�������ăv���C���[�Ƃ̋^���������Z�o����
		int distance = GetPsuedoDistance(FindCandidateRowBelow(colmun, row));
		// �ł��߂��ꍇ��
		if (distance < *bestGuardDistance)
		{
			// �v���C���[�Ƃ̋^���������X�V
			*bestGuardDistance = distance;
			// ���ֈړ�
			moveDirection = MoveDirection::Down;
		}
	}
	// ��ֈړ��\�Ȃ�
	if (IsMovableUp(colmun, row))
	{
		// ��Ɉړ��������̍œK�ȍs�������ăv���C���[�Ƃ̋^���������Z�o����
		int distance = GetPsuedoDistance(FindCandidateRowAbove(colmun, row));
		// �ł��߂��ꍇ��
		if (distance < *bestGuardDistance)
		{
			// �v���C���[�Ƃ̋^���������X�V
			*bestGuardDistance = distance;
			// ��ֈړ�
			moveDirection = MoveDirection::UP;
		}
	}

	return moveDirection;
}

// ���ֈړ����邩���ׂ�֐�
//
// �ړ��\�Ȋe�s�̏㉺�������ł��v���C���[�Ƃ̋^���������߂������ֈړ�����
Guard::MoveDirection Guard::SelectMoveLeftAndRight(int* bestGuardDistance, int colmunLimit, Direction direction)
{
	MoveDirection moveDirection = MoveDirection::None;

	// �K�[�h�̈ʒu
	int colmun = m_tilePosition.x;
	int row = m_tilePosition.y;

	// ���݂̃K�[�h�̈ʒu�܂Œ��ׂ��̂ō��ֈړ��s��
	while (colmun != colmunLimit)
	{
		// ���Ɉړ��\�Ȃ�
		if (IsMovableDown(colmunLimit, row))
		{
			// ���Ɉړ��������̍œK�ȍs�������ăv���C���[�Ƃ̋^���������Z�o����
			int distance = GetPsuedoDistance(FindCandidateRowBelow(colmunLimit, row));
			// �ł��߂��ꍇ��
			if (distance < *bestGuardDistance)
			{
				// �v���C���[�Ƃ̋^���������X�V
				*bestGuardDistance = distance;
				if (direction == Direction::Left)
				{
					// ���ֈړ�
					moveDirection = MoveDirection::Left;
				}
				else
				{
					// �E�ֈړ�
					moveDirection = MoveDirection::Right;
				}
			}
		}

		// ��ֈړ��\�Ȃ�
		if (IsMovableUp(colmunLimit,row))
		{
			// ��Ɉړ��������̍œK�ȍs�������ăv���C���[�Ƃ̋^���������Z�o����
			int distance = GetPsuedoDistance(FindCandidateRowAbove(colmunLimit, row));
			// �ł��߂��ꍇ��
			if (distance < *bestGuardDistance)
			{
				// �v���C���[�Ƃ̋^���������X�V
				*bestGuardDistance = distance;
				if (direction == Direction::Left)
				{
					// ���ֈړ�
					moveDirection = MoveDirection::Left;
				}
				else
				{
					// �E�ֈړ�
					moveDirection = MoveDirection::Right;
				}
			}
		}

		if (direction == Direction::Left)
		{
			// �P�E�̗�֒��ׂ�ʒu���ړ�
			colmunLimit++;
		}
		else
		{
			// �P���̗�֒��ׂ�ʒu���ړ�
			colmunLimit--;
		}
	}

	return moveDirection;
}

// ���Ɉړ��\�ȃ^�C�������ׂ�֐�
bool Guard::IsMovableDown(int colmun, int row)
{
	if ( (row != Level::MAX_GAME_ROW)	// �P�ԉ��̍s�ł͂Ȃ�
	  && (m_pLevel->GetTilePage2(colmun, row + 1) != Tile::Type::Blick)	// �����K�łȂ�
	  && (m_pLevel->GetTilePage2(colmun, row + 1) != Tile::Type::Stone)	// �΂łȂ�
	   )
	{
		return true;
	}
	return false;
}

// ��Ɉړ��\�ȃ^�C�������ׂ�֐�
bool Guard::IsMovableUp(int colmun, int row)
{
	if ( (row != 0)	// ��ԏ�̍s�łȂ�
	  && (m_pLevel->GetTilePage2(colmun, row) == Tile::Type::Ladder)	// �n�V�S������
	   )
	{
		return true;
	}
	return false;
}

// �ړ��\�ȃ^�C�������ׂ�֐��i�����j
bool Guard::IsMovableTileFall(Tile::Type page1, Tile::Type page2)
{
	// �󔒁A�v���C���[�Ȃ痎��
	if ((page1 == Tile::Type::Empty) || (page1 == Tile::Type::Player))
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

//�X�e�[�g���X�V���ċ���𗎂Ƃ�����
void Guard::UpdateGoldDropTimer()
{
	// �s���^�C�}�[���O�ɂȂ����痎�Ƃ�
	if (m_actionStateTimer < 0)
	{
		m_actionStateTimer++;
		if (m_actionStateTimer == 0)
		{
			// �󔒂Ȃ����𗎂Ƃ�
			if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Empty)
			{
				m_pLevel->SetTilePage2(m_tilePosition.x, m_tilePosition.y, Tile::Type::Gold);
			}
			else
			{
				// ���̋@���҂�
				m_actionStateTimer--;
			}
		}
	}
}

// �v���C���[�Ƃ̋^�����������߂�֐�
//
// �������߂��قǑI������₷���Ȃ�
// �I������₷������
//   �P�E�v���C���[�������s
//   �Q�E�v���C���[���� (+100)
//   �R�E�v���C���[���� (+200)
int Guard::GetPsuedoDistance(int row)
{
	// �v���C���[�̈ʒu
	int playerColmun = m_pGamePlayScene->GetPlayer()->GetTilePosition().x;
	int playerRow = m_pGamePlayScene->GetPlayer()->GetTilePosition().y;

	// �����s�ɂ���ꍇ
	if (row == playerRow)
	{
		// �v���C���[�Ƃ̋�����Ԃ�
		return abs(playerColmun - m_tilePosition.x);
	}
	// �v���C���[����ɂ���
	else if(row > playerRow)
	{
		return 200 + (row - playerRow);
	}
	// �v���C���[�����ɂ���
	return 100 + (playerRow - row);
}

// �ړ��\�ȃ^�C�������ׂ�֐��i�㍶�E�j
bool Guard::IsMovableTileULR(Tile::Type tile)
{
	if ((tile == Tile::Type::Blick)	// page1:�����K
		|| (tile == Tile::Type::Stone)	// page1:��
		|| (tile == Tile::Type::Guard)	// page1:�K�[�h
		|| (tile == Tile::Type::Trap)	// page1:㩁i㩂͏ォ��ȊO�͈ړ��s�j
		)
	{
		// �ړ��s��
		return false;
	}
	// �ړ��\
	return true;
}

// �ړ��\�ȃ^�C�������ׂ�֐��i���j
bool Guard::IsMovableTileD(Tile::Type tile)
{
	if ((tile == Tile::Type::Blick)	// page1:�����K
		|| (tile == Tile::Type::Stone)	// page1:��
		|| (tile == Tile::Type::Guard)	// page1:�K�[�h
		)
	{
		// �ړ��s��
		return false;
	}
	// �ړ��\
	return true;
}


