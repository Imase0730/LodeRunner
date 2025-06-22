//--------------------------------------------------------------------------------------
// File: ScoreRankingScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/StringRenderer.h"
#include "Game/NumberRenderer.h"
#include "Game/Blink.h"

// �N���X�̑O���錾
class Game;

// �Q�[���v���C�V�[��
class ScoreRankingScene
{
	// �񋓌^�̒�` -----------------------------------------------------
public:

	// ���[�h
	enum class Mode
	{
		Display,	// �X�R�A�\��
		Entry,		// �X�R�A�o�^
	};

// �N���X�萔�̐錾 -------------------------------------------------
private:

	// �_�ł̊Ԋu
	static constexpr int BLINK_INTERVAL = 16;

	// ��ʐ؂�ւ����̃E�G�C�g
	static constexpr int TRANSITION_DELAY_FRAMES = 180;

// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// ���̃V�[�����܂ރQ�[���I�u�W�F�N�g�ւ̃|�C���^
	Game* m_pGame;

	// ���[�h
	Mode m_mode;

	// ����
	StringRenderer m_stringRenderer;

	// ����
	NumberRenderer m_numberRenderer;

	// �C�j�V�����̕���
	StringRenderer m_initialStringRenderer;

	// ���x���̐���
	NumberRenderer m_levelNumberRenderer;

	// �X�R�A�̐���
	NumberRenderer m_scoreNumberRenderer;

	// �X�R�A�o�^�ʒu
	int m_entryIndex;

	// �_��
	Blink m_blink;

	// ���͒��̕���
	char m_inputCharacter;

	// �I�����̉�ʐ؂�ւ��E�G�C�g�^�C�}�[
	int m_waitTimer;

// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	ScoreRankingScene(Game* pGame);

	// �f�X�g���N�^
	~ScoreRankingScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update(int keyCondition, int keyTrigger);

	// �`�揈��
	void Render(int ghTileset);

	// �I������
	void Finalize();

private:

	// �o�^����ʒu���擾����֐�
	int GetScoreIndex(int score);

public:

	// �f�o�b�O����\������֐�
	void DisplayDebugInformation(int offsetX, int offsetY) const;

};
