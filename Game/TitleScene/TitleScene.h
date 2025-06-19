//--------------------------------------------------------------------------------------
// File: TitleScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/StringRenderer.h"
#include "Game/Blink.h"
#include "Game/Level.h"
#include "Game/Tile.h"

// �N���X�̑O���錾
class Game;

// �Q�[���v���C�V�[��
class TitleScene
{
	// �񋓌^�̒�` -----------------------------------------------------
public:

	// ���[�h
	enum class Mode
	{
		Game,	// �Q�[��
		Edit,	// �G�f�B�b�g
		Score,	// �X�R�A
	};

// �N���X�萔�̐錾 -------------------------------------------------
private:
	
	// �^�C�g���̈ʒu
	static constexpr POINT TITLE_POSITION{ (Screen::GAME_WIDTH - Tile::TILE_PIXEL_WIDTH * 11) / 2, Tile::TILE_PIXEL_HEIGHT * 2 };

	// GAME�̈ʒu
	static constexpr POINT GAME_POSITION{ (Screen::GAME_WIDTH - Tile::TILE_PIXEL_WIDTH * 4) / 2, Tile::TILE_PIXEL_HEIGHT * 6 };

	// EDIT�̈ʒu
	static constexpr POINT EDIT_POSITION{ (Screen::GAME_WIDTH - Tile::TILE_PIXEL_WIDTH * 4) / 2, Tile::TILE_PIXEL_HEIGHT * 9 };

	// SCORE�̈ʒu
	static constexpr POINT SCORE_POSITION{ (Screen::GAME_WIDTH - Tile::TILE_PIXEL_WIDTH * 5) / 2, Tile::TILE_PIXEL_HEIGHT * 12 };

	// �_�ł̊Ԋu
	static constexpr int BLINK_INTERVAL = 60;

// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// ���̃V�[�����܂ރQ�[���I�u�W�F�N�g�ւ̃|�C���^
	Game* m_pGame;

	// �^�C�g���̕�����
	StringRenderer m_titleStringRenderer;

	// Game�̕�����
	StringRenderer m_gameStringRenderer;

	// Edit�̕�����
	StringRenderer m_editStringRenderer;

	// Score�̕�����
	StringRenderer m_scoreStringRenderer;

	// ���[�h
	Mode m_mode;

	// �_�Ő���
	Blink m_blink;

// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	TitleScene(Game* pGame);

	// �f�X�g���N�^
	~TitleScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update(int keyCondition, int keyTrigger);

	// �`�揈��
	void Render(int ghTileset);

	// �I������
	void Finalize();

};
