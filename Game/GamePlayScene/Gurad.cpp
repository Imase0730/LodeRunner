//--------------------------------------------------------------------------------------
// File: Gurad.cpp
//
//--------------------------------------------------------------------------------------
#include "Gurad.h"
#include "GamePlayScene.h"
#include "Game/Level.h"
#include "Player.h"

// �R���X�g���N�^
Gurad::Gurad(GamePlayScene* pScene, Level* pLevel)
	: m_pGamePlayScene{ pScene }
	, m_pLevel{ pLevel }
	, m_isActive{ false }
	, m_isAlive{ false }
	, m_isDisplay{ false }
	, m_tilePosition{ 0, 0 }
	, m_adjustPosition{ 0, 0 }
	, m_faceDirection{ Direction::Left }
	, m_guardAnimationState{ GuradAnimationState::Run01_L }
	, m_goldTimer{ 0 }
	, m_resurrectionTimer{ 0 }
	, m_moveDirection{ MoveDirection::None }
{
}

// �f�X�g���N�^
Gurad::~Gurad()
{
}

// ����������
void Gurad::Initialize(POINT tilePosition, POINT ajustPosition)
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

	// �e�ϐ��̏�����
	m_faceDirection = Direction::Left;
	m_guardAnimationState = GuradAnimationState::Run01_L;
	m_goldTimer = 0;
	m_resurrectionTimer = 0;
	m_moveDirection = MoveDirection::None;
}

// �X�V����
void Gurad::Update()
{
	// ����ێ��^�C�}�[���O���傫���ꍇ�͌��ɗ����Ă�
	// ����ێ��^�C�}�[���|�̏ꍇ�͋���ێ����i�J�E���^�[�Ƃ��Ďg�p�j
	if (m_goldTimer > 0)
	{
		// ����ێ��^�C�}�[�����Z
		m_goldTimer--;

		// ���ɗ���������A���t���[���̓A�j���[�V�������Ȃ�
		if (m_goldTimer > HOLE_ANIMATION_START_FRAME) return;

		// ���ɗh���A�j���[�V����
		int idx = m_goldTimer - (HOLE_ANIMATION_START_FRAME - HOLE_ANIMATION_TABLE_SIZE + 1);
		if (idx >= 0)
		{
			m_adjustPosition.x = HOLE_ANIMATION_TABLE[idx];
			return;
		}
	}
	else
	{
		// ��������
		if (IsFalling())
		{
			// ������
			Falling();
			return;
		}
	}

	// �ړ����������߂�
	m_moveDirection = DecideMoveDirection();

	// �ړ�
	switch (m_moveDirection)
	{
	case Gurad::MoveDirection::UP:
		MoveUp();
		break;
	case Gurad::MoveDirection::Down:
		MoveDown();
		break;
	case Gurad::MoveDirection::Left:
		MoveLeft();
		break;
	case Gurad::MoveDirection::Right:
		MoveRight();
		break;
	default:
		break;
	}
}

// �`�揈��
void Gurad::Render(int ghTileset)
{
	// �\��(OFF)
	if (!m_isDisplay) return;

	// ������
	if (m_resurrectionTimer > 0)
	{
		// �����̃A�j���[�V����
		if (m_resurrectionTimer == RESURRECTION_ANIME_TIME_EGG01) m_guardAnimationState = GuradAnimationState::Egg01;
		if (m_resurrectionTimer == RESURRECTION_ANIME_TIME_EGG02) m_guardAnimationState = GuradAnimationState::Egg02;
	}

	// �K�[�h�̕`��
	POINT pos = GUARD_SPRITES[static_cast<int>(m_guardAnimationState)];
	DrawRectGraph( m_tilePosition.x * Level::TILE_PIXEL_WIDTH + (m_adjustPosition.x - Level::TILE_CENTER_X) * 2
				 , m_tilePosition.y * Level::TILE_PIXEL_HEIGHT + (m_adjustPosition.y - Level::TILE_CENTER_Y) * 2
				 , Level::TILE_PIXEL_WIDTH * pos.x, Level::TILE_PIXEL_HEIGHT * pos.y
				 , Level::TILE_PIXEL_WIDTH, Level::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
}

// �w��ʒu���畜������֐�
void Gurad::Resurrection(int colmun, int row)
{
	// �A�j���[�V������������
	m_guardAnimationState = GuradAnimationState::AnimNone;
	// �ʒu��ݒ�
	SetTilePosition(colmun, row);
	// �����^�C�}�[��ݒ�
	SetResurrectionTimer(RESURRECTION_ANIME_TIME_NONE);
}

// �ʒu���ɒ�������֐�
void Gurad::AjustCloumn()
{
	// ��������K�v�͂Ȃ�
	if (m_adjustPosition.x == Level::TILE_CENTER_X) return;

	// ���E�ɂ���Ă���ꍇ�͒�������
	if (m_adjustPosition.x < Level::TILE_CENTER_X)
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
void Gurad::AjustRow()
{
	// ��������K�v�͂Ȃ�
	if (m_adjustPosition.y == Level::TILE_CENTER_Y) return;

	// �㉺�ɂ���Ă���ꍇ�͒�������
	if (m_adjustPosition.y < Level::TILE_CENTER_Y)
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
bool Gurad::IsFalling()
{
	// �n�V�S�Ȃ�ړ��\
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Ladder) return false;

	// ���[�v�ɂ���ł���Έړ��\
	if ( (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Rope)
	  && (m_adjustPosition.y == Level::TILE_CENTER_Y)
	   )
	{
		return false;
	}

	// �󒆂ɕ����Ă���
	if (m_adjustPosition.y < Level::TILE_CENTER_Y)
	{
		return true;
	}

	// ��ԉ��Ȃ�ړ��\
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// ���̍s�������\�Ȃ�
	Level::Tile page1 = m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y + 1);
	Level::Tile page2 = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y + 1);
	if (IsMovableTileFall(page1, page2)) return true;

	return false;
}

// ��Ɉړ��\�����ׂ�֐�
bool Gurad::IsMovableUp()
{
	// �n�V�S��Ȃ̂Ńn�V�S�̐^�񒆂܂ł͈ړ��\
	if (m_adjustPosition.y > Level::TILE_CENTER_Y) return true;

	// ��ԏ�Ȃ�ړ��s��
	if (m_tilePosition.y == 0) return false;

	// ��̍s���ړ��\�Ȃ�
	Level::Tile page1 = m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y - 1);
	if (IsMovableTileULR(page1)) return true;

	return false;
}

bool Gurad::IsMovableDown()
{
	// ������ɂ���̂ŉ��Ɉړ��\
	if (m_adjustPosition.y < Level::TILE_CENTER_Y) return true;

	// ��ԉ��Ȃ�ړ��s��
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// ���̍s���ړ��\�Ȃ�
	Level::Tile page1 = m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y + 1);
	if (IsMovableTileD(page1)) return true;

	return false;
}

// ���Ɉړ��\�����ׂ�֐�
bool Gurad::IsMovableLeft()
{
	// �܂��������Ɉړ�������]�n������
	if (m_adjustPosition.x > Level::TILE_CENTER_X) return true;

	// �����Ȃ̂ňړ��s��
	if (m_tilePosition.x == 0) return false;

	// �ړ���^�C�����ړ��\�����ׂ�
	Level::Tile page1 = m_pLevel->GetTilePage1(m_tilePosition.x - 1, m_tilePosition.y);
	if (IsMovableTileULR(page1)) return true;

	return false;
}

// �E�Ɉړ��\�����ׂ�֐�
bool Gurad::IsMovableRight()
{
	// �܂��E�����Ɉړ�������]�n������
	if (m_adjustPosition.x < Level::TILE_CENTER_X) return true;

	// �E���Ȃ̂ňړ��s��
	if (m_tilePosition.x == Level::MAX_GAME_COLMUN) return false;

	// �ړ���^�C�����ړ��\�����ׂ�
	Level::Tile page1 = m_pLevel->GetTilePage1(m_tilePosition.x + 1, m_tilePosition.y);
	if (IsMovableTileULR(page1)) return true;

	return false;
}

// ������
void Gurad::Falling()
{
	// ��ɍ��킹��悤��X���W�𒲐�����
	AjustCloumn();

	// ����
	m_adjustPosition.y++;
	if (m_adjustPosition.y > 4)
	{
		// ���̃^�C���ֈړ������̂�Page1�̃^�C����������������
		m_adjustPosition.y = 0;

		Level::Tile page2 = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y);
		if (page2 != Level::Tile::Blick)
		{
			// Page2��Page1�փR�s�[
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, page2);
		}
		else
		{
			// �@��������ʉ�
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Level::Tile::Empty);
		}

		// ���̍s�ֈړ�
		m_tilePosition.y++;

		// �v���C���[�Ȃ玀�S
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}

		// �@�������Ȃ�
		if ( (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Blick)
		  && (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Empty)
		   )
		{
			// ����������Ă���
			if (m_goldTimer < 0)
			{
				// ����𗎂Ƃ�
				m_goldTimer = 0;
				// ���򂪒u���邩�H
				if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y - 1) == Level::Tile::Empty)
				{
					// �������ɒu��
					m_pLevel->SetTilePage2(m_tilePosition.x, m_tilePosition.y - 1, Level::Tile::Gold);
				}
				else
				{
					// ��������X�g����
					m_pLevel->LostGold();
				}
			}
		}

		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Level::Tile::Guard);
	}
	else
	{
		// �������̃^�C���`�F�b�N������
		if (m_adjustPosition.y == Level::TILE_CENTER_Y)
		{
			// ���򂪏E���邩�`�F�b�N
			CheckGoldPickedUp();

			// �@�������Ȃ�
			if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Blick)
			{
				// ����������Ă���΃��X�g����
				if (m_goldTimer < 0) m_pLevel->LostGold();

				// ����ێ��^�C�}�[�ݒ�i�����p�Ɏg�p����j
				m_goldTimer = GOLD_TIMER_START_VALUE;

				// ���_�����Z�i�V�T�_�j
				m_pGamePlayScene->AddScore(GamePlayScene::GUARD_HOLE_SCORE);
			}
		}
	}

	// �����A�j���[�V����
	if (m_faceDirection == Direction::Left)
	{
		m_guardAnimationState = GuradAnimationState::Fall_L;
	}
	else
	{
		m_guardAnimationState = GuradAnimationState::Fall_R;
	}
}

// �ړ����������߂�֐�
Gurad::MoveDirection Gurad::DecideMoveDirection()
{
	// ���̒�
	if ( (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Blick)
	  && (m_goldTimer > 0)
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
	int bestGuradDistance = 255;

	// �ǂ̕����ֈړ����ׂ������߂�
	MoveDirection moveDirection = MoveDirection::None;
	MoveDirection dir{};
	dir = SelectMoveUpAndDown(&bestGuradDistance);
	if (dir != MoveDirection::None) moveDirection = dir;
	dir = SelectMoveLeftAndRight(&bestGuradDistance, leftColmunLimit, Direction::Left);
	if (dir != MoveDirection::None) moveDirection = dir;
	dir = SelectMoveLeftAndRight(&bestGuradDistance, rightColmunLimit, Direction::Right);
	if (dir != MoveDirection::None) moveDirection = dir;

	return moveDirection;
}

// ��Ɉړ�
void Gurad::MoveUp()
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
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}
		// Page1�ɏ�������
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Level::Tile::Guard);
	}

	// ���򂪏E���邩�`�F�b�N
	CheckGoldPickedUp();

	// ����ێ��^�C�}�[���X�V���ċ���𗎂Ƃ�����
	UpdateGoldDropTimer();

	// �n�V�S��o��A�j���[�V����
	SetGuradAnimationState(GuradAnimationState::Climb01, GuradAnimationState::Climb02);
}

// ���Ɉړ�
void Gurad::MoveDown()
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
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}
		// Page1�ɏ�������
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Level::Tile::Guard);
	}

	// ���򂪏E���邩�`�F�b�N
	CheckGoldPickedUp();

	// ����ێ��^�C�}�[���X�V���ċ���𗎂Ƃ�����
	UpdateGoldDropTimer();

	// �n�V�S��o��A�j���[�V����
	SetGuradAnimationState(GuradAnimationState::Climb01, GuradAnimationState::Climb02);
}

// ���Ɉړ�
void Gurad::MoveLeft()
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
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}
		// Page1�ɏ�������
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Level::Tile::Guard);
	}

	// ���򂪏E���邩�`�F�b�N
	CheckGoldPickedUp();

	// ����ێ��^�C�}�[���X�V���ċ���𗎂Ƃ�����
	UpdateGoldDropTimer();

	// ���[�v�̏ꍇ
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Rope)
	{
		// ���[�v�ňړ�����A�j���[�V����
		SetGuradAnimationState(GuradAnimationState::Rope01_L, GuradAnimationState::Rope03_L);
	}
	else
	{
		// ����A�j���[�V����
		SetGuradAnimationState(GuradAnimationState::Run01_L, GuradAnimationState::Run03_L);
	}
}

// �E�Ɉړ�
void Gurad::MoveRight()
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
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}
		// Page1�ɏ�������
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Level::Tile::Guard);
	}

	// ���򂪏E���邩�`�F�b�N
	CheckGoldPickedUp();

	// ����ێ��^�C�}�[���X�V���ċ���𗎂Ƃ�����
	UpdateGoldDropTimer();

	// ���[�v�̏ꍇ
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Rope)
	{
		// ���[�v�ňړ�����A�j���[�V����
		SetGuradAnimationState(GuradAnimationState::Rope01_R, GuradAnimationState::Rope03_R);
	}
	else
	{
		// ����A�j���[�V����
		SetGuradAnimationState(GuradAnimationState::Run01_R, GuradAnimationState::Run03_R);
	}
}

// �K�[�h�A�j���[�V�����X�e�[�g�̐ݒ�
void Gurad::SetGuradAnimationState(GuradAnimationState beginAnimState, GuradAnimationState endAnimState)
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

	m_guardAnimationState = static_cast<GuradAnimationState>(animState);
}

// ���򂪏E���邩���ׂ�֐�
void Gurad::CheckGoldPickedUp()
{
	// �^�C���̒��S�łȂ���΃`�F�b�N���Ȃ�
	if (m_adjustPosition.x != Level::TILE_CENTER_X) return;
	if (m_adjustPosition.y != Level::TILE_CENTER_Y) return;

	// ���򂪂Ȃ�
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) != Level::Tile::Gold) return;

	// ���ɋ���������Ă���
	if (m_goldTimer < 0) return;

	// ���򂪂���΋�����E���i����ێ��^�C�}�[��ݒ�j
	m_goldTimer = -0x53;

	// �󔒂ɂ���
	m_pLevel->SetTilePage2(m_tilePosition.x, m_tilePosition.y, Level::Tile::Empty);
}

// �w�肵���s�܂ňړ��\�����ׂ�֐�
bool Gurad::IsMovableColumn(int column)
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
		if ( (m_pLevel->GetTilePage2(guardColumn, guardRow) == Level::Tile::Ladder)
		  || (m_pLevel->GetTilePage2(guardColumn, guardRow) == Level::Tile::Rope)
		   )
		{
			continue;
		}
		// �����󔒂�㩂łȂ��Ȃ�ړ��\
		if ( (m_pLevel->GetTilePage2(guardColumn, guardRow + 1) != Level::Tile::Empty)
		  && (m_pLevel->GetTilePage2(guardColumn, guardRow + 1) != Level::Tile::Trap)
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
void Gurad::GetLeftRightLimits(int* colmun, Direction direction, int limit)
{
	*colmun = m_tilePosition.x;
	int row = m_tilePosition.y;

	int move = 1;
	if (direction == Direction::Left) move = -1;

	while (*colmun != limit)
	{
		// �����K�܂��͐΂Ȃ�I��
		if ( (m_pLevel->GetTilePage1(*colmun + move, row) == Level::Tile::Blick)		// �����K
		  || (m_pLevel->GetTilePage1(*colmun + move, row) == Level::Tile::Stone)		// ��
		   )
		{
			break;
		}

		// �n�V�S�܂��̓��[�v�łȂ�
		if ( (m_pLevel->GetTilePage1(*colmun + move, row) != Level::Tile::Ladder)		// �n�V�S
		  && (m_pLevel->GetTilePage1(*colmun + move, row) != Level::Tile::Rope)			// ���[�v
		   )
		{
			// �s����ԉ��łȂ�
			if (row != Level::MAX_GAME_ROW)
			{
				// ���������K�A�΁A�n�V�S�łȂ��Ȃ�I��
				if ( (m_pLevel->GetTilePage2(*colmun + move, row + 1) != Level::Tile::Blick)	// �����K
				  && (m_pLevel->GetTilePage2(*colmun + move, row + 1) != Level::Tile::Stone)	// ��
				  && (m_pLevel->GetTilePage2(*colmun + move, row + 1) != Level::Tile::Ladder)	// �n�V�S
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
int Gurad::FindCandidateRowBelow(int colmun, int row)
{
	// �v���C���[�̂���s
	int playerRow = m_pGamePlayScene->GetPlayer()->GetTilePosition().y;

	// ��ԉ��̍s�܂Œ��ׂ�
	while (row != Level::MAX_GAME_ROW)
	{
		// �P���̍s�������K���΂Ȃ�I��
		if ( (m_pLevel->GetTilePage2(colmun, row + 1) == Level::Tile::Blick)	// �����K
		  || (m_pLevel->GetTilePage2(colmun, row + 1) == Level::Tile::Stone)	// ��
			)
		{
			return row;
		}
		// �P�s����
		row++;
		// �󔒂łȂ�
		if (m_pLevel->GetTilePage2(colmun, row) != Level::Tile::Empty)
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
int Gurad::FindCandidateRowAbove(int colmun, int row)
{
	// �v���C���[�̂���s
	int playerRow = m_pGamePlayScene->GetPlayer()->GetTilePosition().y;

	// ��ԏ�̍s�܂Œ��ׂ�
	while (row != 0)
	{
		// �n�V�S�łȂ��̂ŏ�Ɉړ��ł��Ȃ��̂ŏI��
		if (m_pLevel->GetTilePage2(colmun, row) != Level::Tile::Ladder)
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
bool Gurad::CheckLeftRightMove(int colmun, int row, Direction direction)
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

	if (m_pLevel->GetTilePage2(colmun, row) == Level::Tile::Rope)	// ���[�v
	{
		return true;
	}
	else
	{
		// �����������K�A�΁A�n�V�S�Ȃ�
		if ( (m_pLevel->GetTilePage2(colmun, row + 1) == Level::Tile::Blick)	// �����K
		  || (m_pLevel->GetTilePage2(colmun, row + 1) == Level::Tile::Stone)	// ��
		  || (m_pLevel->GetTilePage2(colmun, row + 1) == Level::Tile::Ladder)	// �n�V�S
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
Gurad::MoveDirection Gurad::SelectMoveUpAndDown(int* bestGuradDistance)
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
		if (distance < *bestGuradDistance)
		{
			// �v���C���[�Ƃ̋^���������X�V
			*bestGuradDistance = distance;
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
		if (distance < *bestGuradDistance)
		{
			// �v���C���[�Ƃ̋^���������X�V
			*bestGuradDistance = distance;
			// ��ֈړ�
			moveDirection = MoveDirection::UP;
		}
	}

	return moveDirection;
}

// ���ֈړ����邩���ׂ�֐�
//
// �ړ��\�Ȋe�s�̏㉺�������ł��v���C���[�Ƃ̋^���������߂������ֈړ�����
Gurad::MoveDirection Gurad::SelectMoveLeftAndRight(int* bestGuradDistance, int colmunLimit, Direction direction)
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
			if (distance < *bestGuradDistance)
			{
				// �v���C���[�Ƃ̋^���������X�V
				*bestGuradDistance = distance;
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
			if (distance < *bestGuradDistance)
			{
				// �v���C���[�Ƃ̋^���������X�V
				*bestGuradDistance = distance;
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
bool Gurad::IsMovableDown(int colmun, int row)
{
	if ( (row != Level::MAX_GAME_ROW)	// �P�ԉ��̍s�ł͂Ȃ�
	  && (m_pLevel->GetTilePage2(colmun, row + 1) != Level::Tile::Blick)	// �����K�łȂ�
	  && (m_pLevel->GetTilePage2(colmun, row + 1) != Level::Tile::Stone)	// �΂łȂ�
	   )
	{
		return true;
	}
	return false;
}

// ��Ɉړ��\�ȃ^�C�������ׂ�֐�
bool Gurad::IsMovableUp(int colmun, int row)
{
	if ( (row != 0)	// ��ԏ�̍s�łȂ�
	  && (m_pLevel->GetTilePage2(colmun, row) == Level::Tile::Ladder)	// �n�V�S������
	   )
	{
		return true;
	}
	return false;
}

// �ړ��\�ȃ^�C�������ׂ�֐��i�����j
bool Gurad::IsMovableTileFall(Level::Tile page1, Level::Tile page2)
{
	// �󔒁A�v���C���[�Ȃ痎��
	if ((page1 == Level::Tile::Empty) || (page1 == Level::Tile::Player))
	{
		return true;
	}

	// �K�[�h�Ȃ痎�����Ȃ�
	if (page1 == Level::Tile::Guard)
	{
		return false;
	}

	// �����K�A�΁A�n�V�S�Ȃ痎�����Ȃ�
	if ((page2 == Level::Tile::Blick) || (page2 == Level::Tile::Stone) || (page2 == Level::Tile::Ladder))
	{
		return false;
	}

	return true;
}

// �ړ��\�ȃ^�C�������ׂ�֐��i�㍶�E�j
bool Gurad::IsMovableTileULR(Level::Tile tile)
{
	if ( (tile == Level::Tile::Blick)	// page1:�����K
	  || (tile == Level::Tile::Stone)	// page1:��
	  || (tile == Level::Tile::Guard)	// page1:�K�[�h
	  || (tile == Level::Tile::Trap)	// page1:㩁i㩂͏ォ��ȊO�͈ړ��s�j
	   )
	{
		// �ړ��s��
		return false;
	}
	// �ړ��\
	return true;
}

// �ړ��\�ȃ^�C�������ׂ�֐��i���j
bool Gurad::IsMovableTileD(Level::Tile tile)
{
	if ( (tile == Level::Tile::Blick)	// page1:�����K
	  || (tile == Level::Tile::Stone)	// page1:��
	  || (tile == Level::Tile::Guard)	// page1:�K�[�h
	   )
	{
		// �ړ��s��
		return false;
	}
	// �ړ��\
	return true;
}

// ����ێ��^�C�}�[���X�V���ċ���𗎂Ƃ�����
void Gurad::UpdateGoldDropTimer()
{
	// ����ێ��^�C�}�[���O�ɂȂ����痎�Ƃ�
	if (m_goldTimer < 0)
	{
		m_goldTimer++;
		if (m_goldTimer == 0)
		{
			// �󔒂Ȃ����𗎂Ƃ�
			if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Empty)
			{
				m_pLevel->SetTilePage2(m_tilePosition.x, m_tilePosition.y, Level::Tile::Gold);
			}
			else
			{
				// ���̋@���҂�
				m_goldTimer--;
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
int Gurad::GetPsuedoDistance(int row)
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


