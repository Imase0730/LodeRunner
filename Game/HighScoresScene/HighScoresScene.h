//--------------------------------------------------------------------------------------
// File: HighScoresScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/StringRenderer.h"
#include "Game/NumberRenderer.h"

// �N���X�̑O���錾
class Game;

// �Q�[���v���C�V�[��
class HighScoresScene
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

// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	HighScoresScene(Game* pGame);

	// �f�X�g���N�^
	~HighScoresScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update(int keyCondition, int keyTrigger);

	// �`�揈��
	void Render(int ghTileset);

	// �I������
	void Finalize();

};
