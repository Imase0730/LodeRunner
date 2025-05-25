//--------------------------------------------------------------------------------------
// File: GamePlayScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/Stage.h"
#include "Game/String.h"
#include "Game/Number.h"

// �N���X�̑O���錾
class Game;

// �Q�[���v���C�V�[��
class GamePlayScene
{
	// �񋓌^�̒�` -----------------------------------------------------
public:

// �N���X�萔�̐錾 -------------------------------------------------
private:
	
	// �L�[���s�[�g�̊Ԋu
	static constexpr int KEY_REPEAT_INTEVAL = 10;

	// �_�ł̊Ԋu
	static constexpr int BLINK_INTEVAL = 60;

// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// ���̃V�[�����܂ރQ�[���I�u�W�F�N�g�ւ̃|�C���^
	Game* m_pGame;

	// �X�e�[�W
	Stage m_stage;

	// SCORE�̕�����
	String m_scoreString;

	// MEN�̕�����
	String m_menString;

	// LEVEL�̕�����
	String m_levelString;

	// �X�R�A�\��
	Number m_scoreNumber;

	// �c�@���\��
	Number m_menNumber;

	// ���x���\��
	Number m_levelNumber;

// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	GamePlayScene(Game* pGame);

	// �f�X�g���N�^
	~GamePlayScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update(int keyCondition, int keyTrigger);

	// �`�揈��
	void Render(int ghTileset);

	// �I������
	void Finalize();

private:


public:


};
