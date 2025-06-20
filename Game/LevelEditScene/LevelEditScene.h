//--------------------------------------------------------------------------------------
// File: LevelEditScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/Level.h"
#include "Game/Blink.h"
#include "Game/NumberRenderer.h"
#include "Game/StringRenderer.h"
#include "Game/Tile.h"
#include "YesNoDialog.h"

// �N���X�̑O���錾
class Game;

// �Q�[���v���C�V�[��
class LevelEditScene
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

	// ���x���̍ő�l
	static constexpr int LEVEL_MAX = 999;

	// �I���\�ȃ^�C���̐�
	static constexpr int SELECT_TILE_MAX = 10;

// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// ���̃V�[�����܂ރQ�[���I�u�W�F�N�g�ւ̃|�C���^
	Game* m_pGame;

	// �ҏW���̃X�e�[�W�ԍ�
	int m_levelId;

	// �X�e�[�W
	Level m_level;

	// �_�Ő���
	Blink m_blink;

	// ��ʉ����̃��[�h
	Mode m_mode;

	// �G�f�B�b�g���[�h�p�J�[�\���ʒu
	POINT m_cursorEdit;

	// �I�𒆂̃^�C��
	Tile::Type m_selectTile;

	// SAVE�̕�����
	StringRenderer m_saveStringRenderer;

	// LOAD�̕�����
	StringRenderer m_loadStringRenderer;

	// LEVEL�̕�����
	StringRenderer m_levelStringRenderer;

	// ���x���\��
	NumberRenderer m_levelNumberRenderer;

	// YesNo�_�C�A���O
	YesNoDialog m_yesNoDialog;

// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	LevelEditScene(Game* pGame);

	// �f�X�g���N�^
	~LevelEditScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update(int keyCondition, int keyTrigger);

	// �`�揈��
	void Render(int ghTileset);

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
