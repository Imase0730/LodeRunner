//--------------------------------------------------------------------------------------
// File: ScoreRankingScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/StringRenderer.h"
#include "Game/NumberRenderer.h"

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
		Game,		// �Q�[��
		Edit,		// �G�f�B�b�g
	};

// �N���X�萔�̐錾 -------------------------------------------------
private:
	
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

};
