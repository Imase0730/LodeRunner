//--------------------------------------------------------------------------------------
// File: Gurad.cpp
//
//--------------------------------------------------------------------------------------
#include "Gurad.h"
#include "GamePlayScene.h"
#include "Game/Tile.h"
#include "Player.h"

// �R���X�g���N�^
Gurad::Gurad(GamePlayScene* pScene, Stage* pStage)
	: m_pGamePlayScene{ pScene }
	, m_pStage{ pStage }
	, m_isActive{ false }
	, m_isAlive{ false }
	, m_isDisplay{ false }
	, m_tilePosition{ 0, 0 }
	, m_adjustPosition{ 0, 0 }
	, m_faceDirection{ Direction::Left }
	, m_guardAnimationState{ GuradAnimationState::Run01_L }
	, m_goldTimer{ 0 }
	, m_resurrectionTimer{ 0 }
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

	// �㉺������ �͂����⃍�[�v������Ώ㉺�ړ��\
	// ���E�� ��Q�����Ȃ���΋߂Â��悤�Ɉړ�
	// �K�[�h���ƃv���C���[�������s�ɂ���
	// OK
	// �n�V�S
	// ���[�v
	// MAX_ROW(�s�j

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






	//// ������ꏊ�������K�Ȃ玀�S
	//if (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Blick)
	//{
	//	// ���S
	//	m_isAlive = false;
	//	// �\��(OFF)
	//	m_isDisplay = false;
	//	return;
	//}

	//// ��������
	//if (IsFalling())
	//{
	//	// ������
	//	Falling();
	//	return;
	//}

	//// ��L�[�������ꂽ
	//if (keyCondition & PAD_INPUT_UP)
	//{
	//	// ��Ɉړ�
	//	MoveUp();
	//	return;
	//}

	//// ���L�[�������ꂽ
	//if (keyCondition & PAD_INPUT_DOWN)
	//{
	//	// ���Ɉړ�
	//	MoveDown();
	//	return;
	//}

	//// ���L�[�������ꂽ
	//if (keyCondition & PAD_INPUT_LEFT)
	//{
	//	// ���Ɉړ�
	//	MoveLeft();
	//	return;
	//}

	//// �E�L�[�������ꂽ
	//if (keyCondition & PAD_INPUT_RIGHT)
	//{
	//	// �E�Ɉړ�
	//	MoveRight();
	//	return;
	//}
}

// �`�揈��
void Gurad::Render(int ghTileset) const
{
	// �\��(OFF)
	if (!m_isDisplay) return;

	// �K�[�h�̕`��
	POINT pos = GUARD_SPRITES[static_cast<int>(m_guardAnimationState)];
	DrawRectGraph( m_tilePosition.x * Tile::TILE_PIXEL_WIDTH + (m_adjustPosition.x - Tile::TILE_CENTER_X) * 2
				 , m_tilePosition.y * Tile::TILE_PIXEL_HEIGHT + (m_adjustPosition.y - Tile::TILE_CENTER_Y) * 2
				 , Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				 , Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
}

// �ʒu���ɒ�������֐�
void Gurad::AjustCloumn()
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
void Gurad::AjustRow()
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
bool Gurad::IsFalling() const
{
	// �n�V�S�Ȃ�ړ��\
	if (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Ladder) return false;

	// ���[�v�ɂ���ł���Έړ��\
	if ( (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Rope)
	  && (m_adjustPosition.y == Tile::TILE_CENTER_Y)
	   )
	{
		return false;
	}

	// �󒆂ɕ����Ă���
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y) return true;

	// ��ԉ��Ȃ�ړ��\
	if (m_tilePosition.y == Stage::STAGE_HEIGHT - 1) return false;

	// ���̍s�������\�Ȃ�
	if (Tile::IsMovableTileFall(m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y + 1))) return true;

	return false;
}

// ��Ɉړ��\�����ׂ�֐�
bool Gurad::IsMovableUp() const
{
	// �n�V�S������H
	if (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Ladder)
	{
		// �n�V�S��Ȃ̂Ńn�V�S�̐^�񒆂܂ł͈ړ��\
		if (m_adjustPosition.y > Tile::TILE_CENTER_Y) return true;

		// ��ԏ�Ȃ�ړ��s��
		if (m_tilePosition.y == 0) return false;

		// ��̍s���ړ��\�Ȃ�
		if (Tile::IsMovableTileULR(m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y - 1))) return true;
	}
	else
	{
		if (m_adjustPosition.y > Tile::TILE_CENTER_Y) return true;
	}
	return false;
}

bool Gurad::IsMovableDown() const
{
	// ������ɂ���̂ŉ��Ɉړ��\
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y) return true;

	// ��ԉ��Ȃ�ړ��s��
	if (m_tilePosition.y == Stage::STAGE_HEIGHT - 1) return false;

	// ���̍s���ړ��\�Ȃ�
	if (Tile::IsMovableTileDown(m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y + 1))) return true;

	return false;
}

// ���Ɉړ��\�����ׂ�֐�
bool Gurad::IsMovableLeft() const
{
	// �܂��������Ɉړ�������]�n������
	if (m_adjustPosition.x > Tile::TILE_CENTER_X) return true;

	// �����Ȃ̂ňړ��s��
	if (m_tilePosition.x == 0) return false;

	// �ړ���^�C�����ړ��\�����ׂ�
	return Tile::IsMovableTileULR(m_pStage->GetTileType(m_tilePosition.x - 1, m_tilePosition.y));
}

// �E�Ɉړ��\�����ׂ�֐�
bool Gurad::IsMovableRight() const
{
	// �܂��E�����Ɉړ�������]�n������
	if (m_adjustPosition.x < Tile::TILE_CENTER_X) return true;

	// �E���Ȃ̂ňړ��s��
	if (m_tilePosition.x == Stage::STAGE_WIDTH - 1) return false;

	// �ړ���^�C�����ړ��\�����ׂ�
	return Tile::IsMovableTileULR(m_pStage->GetTileType(m_tilePosition.x + 1, m_tilePosition.y));
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
		m_tilePosition.y++;
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
			m_tilePosition.y--;
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
			m_tilePosition.y++;
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
			m_tilePosition.x--;
		}

		// ���򂪏E���邩�`�F�b�N
		CheckGoldPickedUp();

		// ���[�v�̏ꍇ
		if (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Rope)
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
			m_tilePosition.x++;
		}

		// ���򂪏E���邩�`�F�b�N
		CheckGoldPickedUp();

		// ���[�v�̏ꍇ
		if (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Rope)
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
	if (m_adjustPosition.x != Tile::TILE_CENTER_X) return;
	if (m_adjustPosition.y != Tile::TILE_CENTER_Y) return;

	// ���򂪂���΋�����E��
	if (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Gold)
	{
		// �󔒂ɂ���
		m_pStage->SetTileType(m_tilePosition.x, m_tilePosition.y, Tile::TileType::Empty);
	}
}

// �w�肵���s�܂ňړ��\�����ׂ�֐�
bool Gurad::IsMovableColumn(int column) const
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
		if ( (m_pStage->GetTileType(guardColumn, guardRow) == Tile::TileType::Ladder)
		  || (m_pStage->GetTileType(guardColumn, guardRow) == Tile::TileType::Rope)
		   )
		{
			continue;
		}
		// �����󔒂�㩂łȂ��Ȃ�ړ��\
		if ( (m_pStage->GetTileType(guardColumn, guardRow + 1) != Tile::TileType::Empty)
		  || (m_pStage->GetTileType(guardColumn, guardRow + 1) != Tile::TileType::Trap)
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

	while (true)
	{
		// ���ׂ郊�~�b�g�l�ɒB������I��
		if (*colmun == limit) break;

		// �����K�܂��͐΂Ȃ�I��
		if ( (m_pStage->GetTileType(*colmun + move, row) == Tile::TileType::Blick)		// �����K
		  || (m_pStage->GetTileType(*colmun + move, row) == Tile::TileType::Stone)		// ��
		   )
		{
			break;
		}

		// �n�V�S�܂��̓��[�v�łȂ�
		if ( (m_pStage->GetTileType(*colmun + move, row) != Tile::TileType::Ladder)		// �n�V�S
		  && (m_pStage->GetTileType(*colmun + move, row) != Tile::TileType::Rope)		// ���[�v
		   )
		{
			// �s����ԉ��łȂ�
			if (row != Stage::STAGE_HEIGHT - 1)
			{
				// ���������K�A�΁A�n�V�S�łȂ��Ȃ�I��
				if ( (m_pStage->GetTileType(*colmun + move, row + 1) != Tile::TileType::Blick)	// �����K
				  && (m_pStage->GetTileType(*colmun + move, row + 1) != Tile::TileType::Stone)	// ��
				  && (m_pStage->GetTileType(*colmun + move, row + 1) != Tile::TileType::Ladder)	// �n�V�S
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
	while (row == Stage::STAGE_HEIGHT - 1)
	{
		// �P���̍s�������K���΂Ȃ�I��
		if ( (m_pStage->GetTileType(colmun, row + 1) == Tile::TileType::Blick)	// �����K
		  || (m_pStage->GetTileType(colmun, row + 1) == Tile::TileType::Stone)	// ��
			)
		{
			return row;
		}
		// �P�s����
		row++;
		// �󔒂łȂ�
		if (m_pStage->GetTileType(colmun, row) != Tile::TileType::Empty)
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
	while (row == 0)
	{
		// �n�V�S�łȂ��̂ŏ�Ɉړ��ł��Ȃ��̂ŏI��
		if (m_pStage->GetTileType(colmun, row) != Tile::TileType::Ladder)
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
	if ((colmun < 0) || (colmun > Stage::STAGE_WIDTH - 1)) return false;

	if (m_pStage->GetTileType(colmun, row) == Tile::TileType::Rope)	// ���[�v
	{
		return true;
	}
	else
	{
		// �����������K�A�΁A�n�V�S�Ȃ�
		if ( (m_pStage->GetTileType(colmun, row + 1) == Tile::TileType::Blick)	// �����K
		  || (m_pStage->GetTileType(colmun, row + 1) == Tile::TileType::Stone)	// ��
		  || (m_pStage->GetTileType(colmun, row + 1) == Tile::TileType::Ladder)	// �n�V�S
		   )
		{
			return true;
		}
	}
	return false;
}

// �㉺�ֈړ����邩�]�����đI������֐�
void Gurad::SelectMoveUpAndDown(int* bestGuradDistance)
{
	// �K�[�h�̈ʒu
	int colmun = m_tilePosition.x;
	int row = m_tilePosition.y;

	// ��ԉ��̍s�łȂ��Ȃ�
	if (row != Stage::STAGE_HEIGHT - 1)
	{
		// �P���̍s�������K���΂łȂ����ֈړ��\�Ȃ�
		if ( (m_pStage->GetTileType(colmun, row + 1) != Tile::TileType::Blick)	// �����K
		  && (m_pStage->GetTileType(colmun, row + 1) != Tile::TileType::Stone)	// ��
		   )
		{
			// ���Ɉړ��������̍œK�ȍs�������ăv���C���[�Ƃ̋^���������Z�o����
			int distance = GetPseudoDistance(colmun, FindCandidateRowBelow(colmun, row));
			// �ł��߂��ꍇ��
			if (distance < *bestGuradDistance)
			{
				// �v���C���[�Ƃ̋^���������X�V
				*bestGuradDistance = distance;
				// �s�������ֈړ���
			}
		}
	}
	// ��ԏ�łȂ��Ȃ�
	if (row != 0)
	{
		// �n�V�S�������ֈړ��\�Ȃ�
		if (m_pStage->GetTileType(colmun, row) == Tile::TileType::Ladder)	// �n�V�S
		{
			// ��Ɉړ��������̍œK�ȍs�������ăv���C���[�Ƃ̋^���������Z�o����
			int distance = GetPseudoDistance(colmun, FindCandidateRowAbove(colmun, row));
			// �ł��߂��ꍇ��
			if (distance < *bestGuradDistance)
			{
				// �v���C���[�Ƃ̋^���������X�V
				*bestGuradDistance = distance;
				// �s������ֈړ���
			}
		}
	}
}

// �v���C���[�Ƃ̋^�����������߂�֐�
int Gurad::GetPseudoDistance(int colmun, int row)
{
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


