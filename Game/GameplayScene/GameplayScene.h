//--------------------------------------------------------------------------------------
// File: GamePlayScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/Level.h"
#include "Game/String.h"
#include "Game/Number.h"
#include "Player.h"
//#include "Gurad.h"

// �N���X�̑O���錾
class Game;
class IrisWipe;

// �Q�[���v���C�V�[��
class GamePlayScene
{
	// �񋓌^�̒�` -----------------------------------------------------
public:

// �N���X�萔�̐錾 -------------------------------------------------
private:
	
	// �v���C���[�̐�
	static constexpr int MEN_COUNT = 2;

	// �X�V�Ԋu�i�Q�t���[���ɂP��X�V�j
	static constexpr int UPDATE_INTERVAL = 2;

	// �K�[�h�̍s���t�F�[�Y�̐�
	static constexpr int GUARD_PHASE_COUNT = 3;

	// �K�[�h�̂P�t���[���ɍs������l���̃p�^�[���̃��X�g
	static constexpr int GUARD_PATTERNS_LIST[GUARD_PHASE_COUNT * (Level::GUARD_MAX + 1)] =
	{
		0, 0, 0,	// �K�[�h�̐l���i0�l�j
		0, 1, 1,	// �K�[�h�̐l���i1�l�j
		1, 1, 1,	// �K�[�h�̐l���i2�l�j
		1, 2, 1,	// �K�[�h�̐l���i3�l�j
		1, 2, 2,	// �K�[�h�̐l���i4�l�j
		2, 2, 2,	// �K�[�h�̐l���i5�l�j
	};

public:

	// ����̓��_
	static constexpr int GOLD_SCORE = 250;

// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// ���̃V�[�����܂ރQ�[���I�u�W�F�N�g�ւ̃|�C���^
	Game* m_pGame;

	// �X�V�J�E���^�[
	int m_updateCounter;

	// �X�e�[�W
	Level m_level;

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

	// �X�R�A
	int m_score;

	// �c�@��
	int m_men;

	// ���x��
	int m_levelNo;

	// �v���C���[
	Player m_player;

	//// �K�[�h
	//Gurad* m_pGurad[Level::GUARD_MAX];

	// �K�[�h�̍s���l���̃p�^�[��
	int m_guradPattern[GUARD_PHASE_COUNT];

	// �K�[�h�̍s���l���̃t�F�[�Y�i0�`2�j
	int m_guradPhase;

	// �s�����̃K�[�h�̔ԍ�
	int m_guradNumber;

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

	// �Q�[���X�^�[�g���̏�����
	void GameInitialize();

	// ���x���N���A�����ׂ�֐�
	bool IsLevelCleared();

	// �K�[�h�̍X�V����
	void GuradsUpdate();

public:

	// ���_�����Z����֐�
	void AddScore(int score);

	//// �v���C���[���擾����֐�
	//Player* GetPlayer() { return &m_player; }

};
