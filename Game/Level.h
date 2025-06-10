//--------------------------------------------------------------------------------------
// File: Level.h
//
//--------------------------------------------------------------------------------------
#pragma once

class Level
{
	// �񋓌^�̐錾 -------------------------------------------------
public:

	// ���[�h
	enum class Mode
	{
		GamePlay,
		LevelEdit,
	};

	// �^�C��
	enum class Tile
	{
		Empty,				// �Ȃ�
		Blick,				// �����K
		Stone,				// ��
		Ladder,				// �n�V�S
		Rope,				// ���[�v
		Trap,				// �g���b�v
		InvisibleLadder,	// �B��n�V�S
		Gold,				// ����
		Guard,				// �K�[�h
		Player,				// �v���C���[
	};

	// �@��A�j���[�V�����X�e�[�g
	enum class DigAnimationState
	{
		NotDigging = -1,
		Dig01, Dig02, Dig03, Dig04, Dig05, Dig06,
		Dig07, Dig08, Dig09, Dig10, Dig11, Dig12,
		Fill01, Fill02,
	};

	// �\���̂̐錾 -------------------------------------------------
private:

	// �@���������K�̏��
	struct DigBrick
	{
		POINT position;	// �ʒu
		int timer;		// �����^�C�}�[
	};

	// �N���X�萔�̐錾 -------------------------------------------------
public:

	// �X�e�[�W�f�[�^�̃p�X
	static constexpr char LEVEL_DATA_PATH[] = "Resources/LevelData/";

	// �^�C���̃s�N�Z���T�C�Y�i�\���p�j
	static constexpr int TILE_PIXEL_WIDTH = 10;
	static constexpr int TILE_PIXEL_HEIGHT = 11;

	// �^�C���̃T�C�Y�i�v���O������ł̍��W�v�Z�p�j
	static constexpr int TILE_WIDTH = 5;
	static constexpr int TILE_HEIGHT = 5;

	// �^�C���̒��S�܂ł̋����i�v���O������ł̍��W�v�Z�p�j
	static constexpr int TILE_CENTER_X = 2;
	static constexpr int TILE_CENTER_Y = 2;

	// ���x���T�C�Y
	static constexpr int MAX_GAME_COLMUN = 28 - 1;
	static constexpr int MAX_GAME_ROW = 16 - 1;

	// �X�e�[�W�T�C�Y
	static constexpr int LEVEL_SCREEN_WIDTH = (MAX_GAME_COLMUN + 1) * TILE_PIXEL_WIDTH;
	static constexpr int LEVEL_SCREEN_HEIGHT = (MAX_GAME_ROW + 1) * TILE_PIXEL_HEIGHT;

	// �X�e�[�W��ɒu����K�[�h�̍ő吔
	static constexpr int GUARD_MAX = 5;

	// �@���������K�����L�����郏�[�N�̍ő吔
	static constexpr int DIG_BRICK_MAX = 30;

	// �@���������K�̕�������܂ł̃t���[����
	static constexpr int BRICK_FILL_FRAME = 180;

	// �����K�̕����A�j���[�V�����p
	static constexpr int BRICK_ANIME_TIME_FILL01 = 20;
	static constexpr int BRICK_ANIME_TIME_FILL02 = 10;

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

	// �@���Ă��郌���K�̊G�̈ʒu
	static constexpr POINT DIG_BRICK_SPRITES[] =
	{
		{ 0, 10 }, { 0, 10 },
		{ 1, 10 }, { 1, 10 },
		{ 2, 10 }, { 2, 10 },
		{ 3, 10 }, { 3, 10 },
		{ 4, 10 }, { 4, 10 },
		{ 5, 10 }, { 5, 10 },

		{ 6, 10 }, { 7, 10 },
	};

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �ǂݍ��񂾃��x���f�[�^
	Tile m_loadData[MAX_GAME_ROW + 1][MAX_GAME_COLMUN + 1];

	// PAGE1
	Tile m_page1[MAX_GAME_ROW + 1][MAX_GAME_COLMUN + 1];

	// PAGE2
	Tile m_page2[MAX_GAME_ROW + 1][MAX_GAME_COLMUN + 1];

	// ���[�h
	Mode m_mode;

	// ���x���ԍ�
	int m_levelNo;

	// �X�e�[�W��̃K�[�h�̐�
	int m_guardCount;

	// �X�e�[�W��̋���̐�
	int m_goldCount;

	// �v���C���[�̏����ʒu
	POINT m_playerPosition;

	// �K�[�h�̏����ʒu
	POINT m_guardPosition[GUARD_MAX];

	// �@���������K�̏��L�^�p
	DigBrick m_digBrick[DIG_BRICK_MAX];

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
	int GetLevel() const { return m_levelNo; }

	// �^�C����ݒ肷��֐��iPage1�j
	void SetTilePage1(int x, int y, Tile tile) { m_page1[y][x] = tile; }

	// �^�C����ݒ肷��֐��iPage2�j
	void SetTilePage2(int x, int y, Tile tile) { m_page2[y][x] = tile; }

	// �^�C�����擾����֐��iPage1�j
	Tile GetTilePage1(int x, int y) const { return m_page1[y][x]; }

	// �^�C�����擾����֐��iPage2�j
	Tile GetTilePage2(int x, int y) const { return m_page2[y][x]; }

	//// �w��ꏊ�̃^�C���̌@��A�j���[�V������ݒ肷��֐�
	//void SetTileDigAnimationState(int x, int y, Tile::DigAnimationState state) { m_stageData[y][x].SetDigAnimationState(state); }

	// �w�背�x�����Z�[�u����֐�
	bool SaveLevel(int level);

	// �w�背�x�������[�h����֐�
	bool LoadLevel(int level, Mode mode);

	// �n�V�S���o������֐�
	void AppearLadder();

	// �v���C���[�̈ʒu���擾����֐�
	POINT GetPlayerPosition() const { return m_playerPosition; }

	// �w��ʒu�̃����K�𕜌�����
	void SetFillBrick(int x, int y);

	// �X�e�[�W��̋���̐����擾����֐�
	int GetGoldCount() const { return m_goldCount; }

	// �X�e�[�W��̋���̐������炷�֐�
	void GetGold() { m_goldCount--; }

	// �X�e�[�W��̃K�[�h�̐����擾����֐�
	int GetGuardCount() const { return m_guardCount; }

	// �K�[�h�̈ʒu���擾����֐�
	POINT GetGuardPosition(int index) const { return m_guardPosition[index]; }

};

