//--------------------------------------------------------------------------------------
// File: Gurad.cpp
//
//--------------------------------------------------------------------------------------
#include "Gurad.h"
#include "GamePlayScene.h"
#include "Game/Tile.h"

// �R���X�g���N�^
Gurad::Gurad(GamePlayScene* pScene, Stage* pStage)
	: m_pGamePlayScene{ pScene }
	, m_pStage{ pStage }
	, m_isActive{ false }
	, m_isAlive{ false }
	, m_isDisplay{ false }
	, m_tilePosition{ 0, 0 }
	, m_adjustPosition{ 0, 0 }
	, m_faceDirection{ FaceDirection::Left }
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
	m_faceDirection = FaceDirection::Left;
	m_guardAnimationState = GuradAnimationState::Run01_L;
}

// �X�V����
void Gurad::Update()
{

	// �㉺������ �͂����⃍�[�v������Ώ㉺�ړ��\
	// ���E�� ��Q�����Ȃ���΋߂Â��悤�Ɉړ�
	// �K�[�h���ƃv���C���[�������s�ɂ���
	// OK
	// �n�V�S
	// ���[�v
	// MAX_ROW(�s�j

	// �v���C���[�Ɠ����s�ɂ���ꍇ
	// ���E�ֈړ��@�n�V�S�⃍�[�v�͈ړ��Ȃ̂Ŗ���
	// ��ԉ��̍s�łȂ��A������󔒂܂���㩂�����Ȃ�ړ����Ȃ�
	// �����𖞂����Έړ�����



		// ���Ɉړ�
		MoveLeft();
		return;





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
	if (m_faceDirection == FaceDirection::Left)
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
		// ��������
		m_faceDirection = FaceDirection::Left;

		// �s�ɍ��킹��悤��Y���W�𒲐�����
		AjustRow();

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
		// �E������
		m_faceDirection = FaceDirection::Right;

		// �s�ɍ��킹��悤��Y���W�𒲐�����
		AjustRow();

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

