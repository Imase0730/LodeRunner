//--------------------------------------------------------------------------------------
// File: IrisWipe.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Level.h"

// �A�C���X���C�v�N���X
class IrisWipe
{
	// �񋓌^�̐錾 -------------------------------------------------
public:

	// ���[�h
	enum class Mode
	{
		None,
		Open,	// �I�[�v��
		Close,	// �N���[�Y
	};

	// �N���X�萔�̐錾 -------------------------------------------------
private:

	// ���C�v�̒��S�ʒu
	static constexpr int WIPE_CENTER_X = Level::LEVEL_SCREEN_WIDTH / 2;
	static constexpr int WIPE_CENTER_Y = Level::LEVEL_SCREEN_HEIGHT / 2;

	// ���C�v�̃t���[����
	static constexpr int WIPE_COUNTER = 170;

	// ���C�v�̃s�N�Z���̕�
	static constexpr int WIPE_PIXEL_WIDTH = 4;
	static constexpr int WIPE_PIXEL_HEIGHT = 1;

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �}�X�N�f�[�^
	static unsigned char m_maskData[Level::LEVEL_SCREEN_HEIGHT][Level::LEVEL_SCREEN_WIDTH];

	// ���C�v�p�g�}�X�N�p���t���O
	bool m_isUse;

	// ���C�v�p�}�X�N���쒆�t���O
	bool m_isActive;

	// ���C�v���[�h
	Mode m_mode;

	// ���C�v�̃J�E���^�[
	int m_counter;

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	IrisWipe();

	// �f�X�g���N�^
	~IrisWipe();

	// ����������
	void Initialize(Mode mode);

	// �X�V����
	void Update();

	// �`�揈��
	void Render() const;

	// �I������
	void Finalize();

	// ���C�v���J�n����֐�
	void Start(Mode mode);

	// ���[�v�̃��[�h���擾����֐�
	Mode GetMode() const { return m_mode; }

	// ���C�v���쒆�����ׂ�֐�
	bool IsActive() const { return m_isActive; }

private:

	// �J�E���^�[�̍X�V�֐�
	int UpdateCounter();

};
