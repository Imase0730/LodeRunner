//--------------------------------------------------------------------------------------
// File: GameplayScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Stage.h"
#include "Blink.h"

// �N���X�̑O���錾
class Game;

// �Q�[���v���C�V�[��
class GameplayScene
{
	// �񋓌^�̒�` -----------------------------------------------------
public:

	// ���[�h
	enum class Mode
	{
		GamePlay,
		Edit,
	};

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

	// ���[�h
	Mode m_mode;

	// �O���t�B�b�N�n���h��
	int m_ghTileset;

	// �_�Ő���
	Blink m_blink;

// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	GameplayScene(Game* pGame);

	// �f�X�g���N�^
	~GameplayScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update(int keyCondition, int keyTrigger);

	// �`�揈��
	void Render();

	// �I������
	void Finalize();

private:

	// �G�f�B�b�g���[�h�p�J�[�\���ʒu
	POINT m_cursorEdit;

	// �I�𒆂̃^�C��
	Tile::TileType m_selectTile;

public:

	// ���[�h�̎擾�֐�
	Mode GetMode() const { return m_mode; }

};
