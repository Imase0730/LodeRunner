//--------------------------------------------------------------------------------------
// File: IrisWipe.cpp
//
//--------------------------------------------------------------------------------------
#include "IrisWipe.h"

// �}�X�N�f�[�^
unsigned char IrisWipe::m_maskData[Level::LEVEL_SCREEN_HEIGHT][Level::LEVEL_SCREEN_WIDTH]{};

// �R���X�g���N�^
IrisWipe::IrisWipe()
	: m_counter{ 0 }
	, m_isUse{ false }
	, m_isActive{ false }
	, m_mode{ Mode::None }
{
}

// �f�X�g���N�^
IrisWipe::~IrisWipe()
{
	// �I������
	Finalize();
}

// ����������
void IrisWipe::Initialize(Mode mode)
{
	// ���C�v���s���͏������ł��Ȃ�
	if (m_isActive) return;

	// ���[�h��ݒ�
	m_mode = mode;

	// �J�E���^�[�̏�����
	m_counter = 0;

	// �}�X�N��ʂ��쐬���܂�
	if (m_isUse == false)
	{
		CreateMaskScreen();
		m_isUse = true;
	}

	// �}�X�N�f�[�^��������
	for (int i = 0; i < Level::Level::LEVEL_SCREEN_HEIGHT; i++)
	{
		for (int j = 0; j < Level::LEVEL_SCREEN_WIDTH; j++)
		{
			m_maskData[i][j] = 0xff;
		}
	}
}

// �X�V����
void IrisWipe::Update()
{
	if (m_mode == Mode::None) return;

	// ���ŏ�����
	for (int i = 0; i < Level::Level::LEVEL_SCREEN_HEIGHT; i++)
	{
		for (int j = 0; j < Level::LEVEL_SCREEN_WIDTH; j++)
		{
			m_maskData[i][j] = 0xff;
		}
	}

	// �A�C���X���C�v�p�̉~���쐬����
	int radius = UpdateCounter();
	int cx = WIPE_CENTER_X;
	int cy = WIPE_CENTER_Y;
	int r2 = radius * radius;
	for (int y = -radius; y <= radius; y++)
	{
		for (int x = -radius; x <= radius; x++)
		{
			// �~�������K�^�K�^�ɂ݂���
			int xx = (x / WIPE_PIXEL_WIDTH) * WIPE_PIXEL_WIDTH;
			int yy = (y / WIPE_PIXEL_HEIGHT) * WIPE_PIXEL_HEIGHT;
			if ((xx * xx + yy * yy) < r2)
			{
				xx = cx + x;
				yy = cy + y;
				// �z��O����Ȃ����`�F�b�N
				if ( (xx >= 0) && (xx < Level::LEVEL_SCREEN_WIDTH)
				  && (yy >= 0) && (yy < Level::LEVEL_SCREEN_HEIGHT)
				   )
				{
					m_maskData[yy][xx] = 0;
				}
			}
		}
	}

}

// �`�揈��
void IrisWipe::Render() const
{
	if (m_mode == Mode::None) return;

	// �}�X�N�̃f�[�^���}�X�N��ʂɒ��ڕ`�悷��
	DrawMaskToDirectData(0, 0, Level::LEVEL_SCREEN_WIDTH, Level::LEVEL_SCREEN_HEIGHT, m_maskData, DX_MASKTRANS_BLACK);
}

// �I������
void IrisWipe::Finalize()
{
	// �}�X�N��ʂ��폜���܂�
	if (m_isUse)
	{
		DeleteMaskScreen();
		m_isUse = false;
	}
}

// ���C�v���J�n����֐�
void IrisWipe::Start(Mode mode)
{
	Initialize(mode);
	m_isActive = true;
}

// �J�E���^�[�̍X�V�֐�
int IrisWipe::UpdateCounter()
{
	// �}�X�N���쒆�̂ݍX�V���s��
	if (!m_isActive) return 0;

	// ���C�v�����I���H
	if (m_counter == WIPE_COUNTER)
	{
		// �}�X�N���쒆�t���O(OFF)
		m_isActive = false;
		// �I�[�v���̎�
		if (m_mode == Mode::Open)
		{
			// ��ʂ���}�X�N�͍폜����
			m_mode = Mode::None;
			Finalize();
		}
	}
	else
	{
		m_counter++;
	}
	// �N���[�Y�̎��̓J�E���^�[�������t�ɂ���
	if (m_mode == Mode::Close)
	{
		return WIPE_COUNTER - m_counter;
	}
	return m_counter;
}

