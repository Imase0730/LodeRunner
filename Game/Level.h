//--------------------------------------------------------------------------------------
// File: Level.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Screen.h"
#include "Tile.h"

class Level
{
	// �񋓌^�̐錾 -----------------------------------------------------
public:

	// ���[�h
	enum class Mode
	{
		GamePlay,
		LevelEdit,
	};

	// �N���X�萔�̐錾 -------------------------------------------------
public:

	// �X�e�[�W�f�[�^�̃p�X
	static constexpr char LEVEL_DATA_PATH[] = "Resources/LevelData/";

	// ���x���T�C�Y
	static constexpr int MAX_GAME_COLMUN = 28 - 1;
	static constexpr int MAX_GAME_ROW = 16 - 1;

	// �X�e�[�W�T�C�Y
	static constexpr int LEVEL_SCREEN_WIDTH = (MAX_GAME_COLMUN + 1) * Tile::TILE_PIXEL_WIDTH;
	static constexpr int LEVEL_SCREEN_HEIGHT = (MAX_GAME_ROW + 1) * Tile::TILE_PIXEL_HEIGHT;

	// �X�e�[�W��ɒu����K�[�h�̍ő吔
	static constexpr int GUARD_MAX = 5;

	// �X�e�[�W��ɒu����B��n�V�S�̍ő吔
	static constexpr int INVISIBLE_LADDER_MAX = 45;

	// �^�C���̊G�̈ʒu
	static constexpr POINT TILE_SPRITES[] =
	{
		{ 0, 4 },	// �Ȃ�
		{ 1, 4 },	// �����K
		{ 2, 4 },	// ��
		{ 3, 4 },	// �n�V�S
		{ 4, 4 },	// ���[�v
		{ 5, 4 },	// �g���b�v
		{ 6, 4 },	// �B��n�V�S
		{ 7, 4 },	// ����
		{ 8, 4 },	// �K�[�h
		{ 9, 4 },	// �v���C���[
	};

	// ��ʉ����̏��E�C���h�E�̏c�̕\���ʒu
	static constexpr int INFOMATION_Y = Screen::GAME_HEIGHT - Tile::TILE_PIXEL_HEIGHT;

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �ǂݍ��񂾃��x���f�[�^
	Tile::Type m_loadData[MAX_GAME_ROW + 1][MAX_GAME_COLMUN + 1];

	// PAGE1
	Tile::Type m_page1[MAX_GAME_ROW + 1][MAX_GAME_COLMUN + 1];

	// PAGE2
	Tile::Type m_page2[MAX_GAME_ROW + 1][MAX_GAME_COLMUN + 1];

	// ���[�h
	Mode m_mode;

	// ���x���ԍ�
	int m_levelId;

	// �X�e�[�W��̃K�[�h�̐�
	int m_guardCount;

	// �X�e�[�W��̋���̐�
	int m_goldCount;

	// �X�e�[�W��̉B��n�V�S�̐�
	int m_invisibleLadderCount;

	// �v���C���[�̏����ʒu
	POINT m_playerPosition;

	// �K�[�h�̏����ʒu
	POINT m_guardPosition[GUARD_MAX];

	// �B��n�V�S�̏��L�^�p
	POINT m_invisibleLadderPosition[INVISIBLE_LADDER_MAX];

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	Level(Mode mode);

	// �f�X�g���N�^
	~Level();

	// ������
	void Initialize(int levelNo, Mode mode);

	// �X�V����
	void Update();

	// �`�揈��
	void Render(int ghTileset) const;

	// ���x���ԍ��̎擾
	int GetLevel() const { return m_levelId; }

	// �^�C����ݒ肷��֐��iPage1�j
	void SetTilePage1(int x, int y, Tile::Type tile) { m_page1[y][x] = tile; }

	// �^�C����ݒ肷��֐��iPage2�j
	void SetTilePage2(int x, int y, Tile::Type tile) { m_page2[y][x] = tile; }

	// �^�C�����擾����֐��iPage1�j
	Tile::Type GetTilePage1(int x, int y) const { return m_page1[y][x]; }

	// �^�C�����擾����֐��iPage2�j
	Tile::Type GetTilePage2(int x, int y) const { return m_page2[y][x]; }

	// �w�背�x�����Z�[�u����֐�
	bool SaveLevel(int level) const;

	// �w�背�x�������[�h����֐�
	bool LoadLevel(int level, Mode mode);

	// �B���n�V�S���o������֐�
	void AppearLadder();

	// �v���C���[�̈ʒu���擾����֐�
	POINT GetPlayerPosition() const { return m_playerPosition; }

	// �X�e�[�W��̋���̐����擾����֐�
	int GetGoldCount() const { return m_goldCount; }

	// �X�e�[�W��̋���̐������炷�֐�
	void LostGold() { m_goldCount--; }

	// �X�e�[�W��̃K�[�h�̐����擾����֐�
	int GetGuardCount() const { return m_guardCount; }

	// �K�[�h�̈ʒu���擾����֐�
	POINT GetGuardPosition(int index) const { return m_guardPosition[index]; }

	// Page2�̓��e��Page1�Ɏw��ʒu�̃^�C�����R�s�[����֐�
	void CopyPage2toPage1(int x, int y);

};

