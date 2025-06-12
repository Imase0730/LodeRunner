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
}

// �X�V����
void Gurad::Update()
{
	// �v���C���[���擾
	Player* pPlayer = m_pGamePlayScene->GetPlayer();
	int playerColumn = pPlayer->GetTilePosition().x;
	int playerRow = pPlayer->GetTilePosition().y;

	// ��������
	if (IsFalling())
	{
		// ������
		Falling();
		return;
	}

	// �v���C���[�Ɠ����s�ɂ���ꍇ
	if (m_tilePosition.y == playerRow)
	{
		// �v���C���[�̈ʒu�܂ňړ��\���H
		if (IsMovableColumn(playerColumn))
		{
			// ���E�ǂ���Ƀv���C���[�����邩�`�F�b�N����
			if (m_tilePosition.x > playerColumn)
			{
				// ���Ɉړ�
				MoveLeft();
				return;
			}
			else
			{
				// �E�Ɉړ�
				MoveRight();
				return;
			}
		}
	}

	// �ړ����������Z�b�g
	m_moveDirection = MoveDirection::None;

	// �v���C���[�ƃK�[�h�Ƃ̋^���I�ȋ��������Z�b�g
	int bestGuradDistance = 255;

	// ���E�̈ʒu���ׂ�ʒu���擾
	int leftColmunLimit = 0;
	int rightColmunLimit = 0;
	GetLeftRightLimits(&leftColmunLimit, Direction::Left, 0);
	GetLeftRightLimits(&rightColmunLimit, Direction::Right, Level::MAX_GAME_COLMUN);

	// �ǂ̕����ֈړ����ׂ������߂�
	SelectMoveUpAndDown(&bestGuradDistance);
	SelectMoveLeftAndRight(&bestGuradDistance, leftColmunLimit, Direction::Left);
	SelectMoveLeftAndRight(&bestGuradDistance, rightColmunLimit, Direction::Right);

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
void Gurad::Render(int ghTileset) const
{
	// �\��(OFF)
	if (!m_isDisplay) return;

	// �K�[�h�̕`��
	POINT pos = GUARD_SPRITES[static_cast<int>(m_guardAnimationState)];
	DrawRectGraph( m_tilePosition.x * Level::TILE_PIXEL_WIDTH + (m_adjustPosition.x - Level::TILE_CENTER_X) * 2
				 , m_tilePosition.y * Level::TILE_PIXEL_HEIGHT + (m_adjustPosition.y - Level::TILE_CENTER_Y) * 2
				 , Level::TILE_PIXEL_WIDTH * pos.x, Level::TILE_PIXEL_HEIGHT * pos.y
				 , Level::TILE_PIXEL_WIDTH, Level::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
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
	if (m_adjustPosition.y < Level::TILE_CENTER_Y) return true;

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
	// �n�V�S������H
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Ladder)
	{
		// �n�V�S��Ȃ̂Ńn�V�S�̐^�񒆂܂ł͈ړ��\
		if (m_adjustPosition.y > Level::TILE_CENTER_Y) return true;

		// ��ԏ�Ȃ�ړ��s��
		if (m_tilePosition.y == 0) return false;

		// ��̍s���ړ��\�Ȃ�
		Level::Tile page1 = m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y - 1);
		if (IsMovableTileU(page1)) return true;
	}
	else
	{
		if (m_adjustPosition.y > Level::TILE_CENTER_Y) return true;
	}
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
	Level::Tile page2 = m_pLevel->GetTilePage2(m_tilePosition.x - 1, m_tilePosition.y);
	if (IsMovableTileLR(page1, page2)) return true;
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
	Level::Tile page2 = m_pLevel->GetTilePage2(m_tilePosition.x + 1, m_tilePosition.y);
	if (IsMovableTileLR(page1, page2)) return true;
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
		m_adjustPosition.y = 0;
		Level::Tile page2 = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y);
		if (page2 != Level::Tile::Blick)
		{
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, page2);
		}
		else
		{
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Level::Tile::Empty);
		}
		m_tilePosition.y++;
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Level::Tile::Guard);
	}

	// ���򂪏E���邩�`�F�b�N
	CheckGoldPickedUp();

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

// ��Ɉړ�
void Gurad::MoveUp()
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
			Level::Tile tile = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y);
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, tile);
			m_tilePosition.y--;
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Level::Tile::Guard);
		}

		// ���򂪏E���邩�`�F�b�N
		CheckGoldPickedUp();

		// �n�V�S��o��A�j���[�V����
		SetGuradAnimationState(GuradAnimationState::Climb01, GuradAnimationState::Climb02);
	}
}

// ���Ɉړ�
void Gurad::MoveDown()
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
			Level::Tile tile = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y);
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, tile);
			m_tilePosition.y++;
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Level::Tile::Guard);
		}

		// ���򂪏E���邩�`�F�b�N
		CheckGoldPickedUp();

		// �n�V�S��o��A�j���[�V����
		SetGuradAnimationState(GuradAnimationState::Climb01, GuradAnimationState::Climb02);
	}
}

// ���Ɉړ�
void Gurad::MoveLeft()
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
			Level::Tile tile = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y);
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, tile);
			m_tilePosition.x--;
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Level::Tile::Guard);
		}

		// ���򂪏E���邩�`�F�b�N
		CheckGoldPickedUp();

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
}

// �E�Ɉړ�
void Gurad::MoveRight()
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
			Level::Tile tile = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y);
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, tile);
			m_tilePosition.x++;
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Level::Tile::Guard);
		}

		// ���򂪏E���邩�`�F�b�N
		CheckGoldPickedUp();

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

	// ���򂪂���΋�����E��
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Level::Tile::Gold)
	{
		// �󔒂ɂ���
		m_pLevel->SetTilePage2(m_tilePosition.x, m_tilePosition.y, Level::Tile::Empty);
	}
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

	// ��Ԃ����̍s�܂Œ��ׂ�
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
				// �v���C���[�̕��������s�����̍s�Ȃ�
				if (row >= playerRow) return row;
			}
			// �E���͈ړ��\�Ȃ�
			if (CheckLeftRightMove(colmun, row, Direction::Right))
			{
				// �v���C���[�̕��������s�����̍s�Ȃ�
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
			// �v���C���[�̕��������s�����̍s�Ȃ�
			if (row >= playerRow) return row;
		}
		// �E���͈ړ��\�Ȃ�
		if (CheckLeftRightMove(colmun, row, Direction::Right))
		{
			// �v���C���[�̕��������s�����̍s�Ȃ�
			if (row >= playerRow) return row;
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
void Gurad::SelectMoveUpAndDown(int* bestGuradDistance)
{
	// �K�[�h�̈ʒu
	int colmun = m_tilePosition.x;
	int row = m_tilePosition.y;

	// ���Ɉړ��\�Ȃ�
	if (IsMovableDown(colmun, row))
	{
		// ���Ɉړ��������̍œK�ȍs�������ăv���C���[�Ƃ̋^���������Z�o����
		int distance = GetPsuedoDistance(colmun, FindCandidateRowBelow(colmun, row));
		// �ł��߂��ꍇ��
		if (distance < *bestGuradDistance)
		{
			// �v���C���[�Ƃ̋^���������X�V
			*bestGuradDistance = distance;
			// ���ֈړ�
			m_moveDirection = MoveDirection::Down;
		}
	}
	// ��ֈړ��\�Ȃ�
	if (IsMovableUp(colmun, row))
	{
		// ��Ɉړ��������̍œK�ȍs�������ăv���C���[�Ƃ̋^���������Z�o����
		int distance = GetPsuedoDistance(colmun, FindCandidateRowAbove(colmun, row));
		// �ł��߂��ꍇ��
		if (distance < *bestGuradDistance)
		{
			// �v���C���[�Ƃ̋^���������X�V
			*bestGuradDistance = distance;
			// ��ֈړ�
			m_moveDirection = MoveDirection::UP;
		}
	}
}

// ���ֈړ����邩���ׂ�֐�
//
// �ړ��\�Ȋe�s�̏㉺�������ł��v���C���[�Ƃ̋^���������߂������ֈړ�����
void Gurad::SelectMoveLeftAndRight(int* bestGuradDistance, int colmunLimit, Direction direction)
{
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
			int distance = GetPsuedoDistance(colmunLimit, FindCandidateRowBelow(colmunLimit, row));
			// �ł��߂��ꍇ��
			if (distance < *bestGuradDistance)
			{
				// �v���C���[�Ƃ̋^���������X�V
				*bestGuradDistance = distance;
				if (direction == Direction::Left)
				{
					// ���ֈړ�
					m_moveDirection = MoveDirection::Left;
				}
				else
				{
					// �E�ֈړ�
					m_moveDirection = MoveDirection::Right;
				}
			}
		}

		// ��ֈړ��\�Ȃ�
		if (IsMovableUp(colmunLimit,row))
		{
			// ��Ɉړ��������̍œK�ȍs�������ăv���C���[�Ƃ̋^���������Z�o����
			int distance = GetPsuedoDistance(colmunLimit, FindCandidateRowAbove(colmunLimit, row));
			// �ł��߂��ꍇ��
			if (distance < *bestGuradDistance)
			{
				// �v���C���[�Ƃ̋^���������X�V
				*bestGuradDistance = distance;
				if (direction == Direction::Left)
				{
					// ���ֈړ�
					m_moveDirection = MoveDirection::Left;
				}
				else
				{
					// �E�ֈړ�
					m_moveDirection = MoveDirection::Right;
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
}

// ���Ɉړ��\�ȃ^�C�������ׂ�֐�
//
// ���Ɉړ��\�Ftrue
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
//
// ��Ɉړ��\�Ftrue
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
bool Gurad::IsMovableTileLR(Level::Tile page1, Level::Tile page2)
{
	// �u���b�N�A�΁A�K�[�h�A㩂Ȃ�ړ��s��
	if ( (page1 == Level::Tile::Blick)
	  || (page1 == Level::Tile::Stone)
	  || (page1 == Level::Tile::Guard)
	  || (page2 == Level::Tile::Trap)
	   )
	{
		return false;
	}
	return true;
}

// �ړ��\�ȃ^�C�������ׂ�֐��i��j
bool Gurad::IsMovableTileU(Level::Tile page1)
{
	// �u���b�N�A�΁A�K�[�h�A㩂Ȃ�ړ��s��
	if ((page1 == Level::Tile::Blick)
		|| (page1 == Level::Tile::Stone)
		|| (page1 == Level::Tile::Guard)
		|| (page1 == Level::Tile::Trap)
		)
	{
		return false;
	}
	return true;
}

// �ړ��\�ȃ^�C�������ׂ�֐��i���j
bool Gurad::IsMovableTileD(Level::Tile page1)
{
	// �u���b�N�A�΁A�K�[�h�A㩂Ȃ�ړ��s��
	if ((page1 == Level::Tile::Blick)
		|| (page1 == Level::Tile::Stone)
		|| (page1 == Level::Tile::Guard)
		)
	{
		return false;
	}
	return true;
}

// �v���C���[�Ƃ̋^�����������߂�֐�
//
// �������߂��قǑI������₷���Ȃ�
// �I������₷������
//   �P�E�v���C���[�������s
//   �Q�E�v���C���[���� (+100)
//   �R�E�v���C���[���� (+200)
int Gurad::GetPsuedoDistance(int colmun, int row)
{
	// �v���C���[�̈ʒu
	int playerColmun = m_pGamePlayScene->GetPlayer()->GetTilePosition().x;
	int playerRow = m_pGamePlayScene->GetPlayer()->GetTilePosition().y;

	// �����s�ɂ���ꍇ
	if (row == playerRow)
	{
		// �v���C���[�Ƃ̋�����Ԃ�
		return abs(playerColmun - colmun);
	}
	// �v���C���[����ɂ���
	else if(row > playerRow)
	{
		return 200 + (row - playerRow);
	}
	// �v���C���[�����ɂ���
	return 100 + (playerRow - row);
}


