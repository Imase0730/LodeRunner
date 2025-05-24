//--------------------------------------------------------------------------------------
// File: StageEditScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/Stage.h"
#include "Game/Blink.h"
#include "Game/Number.h"
#include "Game/String.h"

// �N���X�̑O���錾
class Game;

// �Q�[���v���C�V�[��
class StageEditScene
{
	// �񋓌^�̒�` -----------------------------------------------------
public:

	// ��ʉ����̃��[�h
	enum class Mode
	{
		SelectTile,	// �^�C���I��
		Save,		// �Z�[�u
		Load,		// ���[�h
	};

// �N���X�萔�̐錾 -------------------------------------------------
private:
	
	// �L�[���s�[�g�̊Ԋu
	static constexpr int KEY_REPEAT_INTEVAL = 10;

	// �_�ł̊Ԋu
	static constexpr int BLINK_INTEVAL = 60;

	// �X�e�[�W�̍ő�l
	static constexpr int STAGE_MAX = 999;

// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// ���̃V�[�����܂ރQ�[���I�u�W�F�N�g�ւ̃|�C���^
	Game* m_pGame;

	// ���x��
	int m_level;

	// �X�e�[�W
	Stage m_stage;

	// �O���t�B�b�N�n���h��
	int m_ghTileset;

	// �_�Ő���
	Blink m_blink;

	// ��ʉ����̃��[�h
	Mode m_mode;

	// �G�f�B�b�g���[�h�p�J�[�\���ʒu
	POINT m_cursorEdit;

	// �I�𒆂̃^�C��
	Tile::TileType m_selectTile;

	// SAVE�̕�����
	String m_saveString;

	// LOAD�̕�����
	String m_loadString;

	// LEVEL�̕�����
	String m_levelString;

	// ���x���\��
	Number m_levelNumber;

// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	StageEditScene(Game* pGame);

	// �f�X�g���N�^
	~StageEditScene();

	// ����������
	void Initialize(int ghTileset);

	// �X�V����
	void Update(int keyCondition, int keyTrigger);

	// �`�揈��
	void Render();

	// �I������
	void Finalize();

private:

	// �ȈՃL�[���s�[�g
	int KeyRepeat(int keyCondition);

	// �^�C���I��
	void SelectTile(int keyCondition, int keyRepeat);

	// �Z�[�u
	void Save(int keyTrigger, int keyRepeat);

	// ���[�h
	void Load(int keyTrigger, int keyRepeat);

};
