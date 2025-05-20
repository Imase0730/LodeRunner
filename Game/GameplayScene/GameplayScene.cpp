//--------------------------------------------------------------------------------------
// File: GameplayScene.cpp
//
//--------------------------------------------------------------------------------------
#include "GameplayScene.h"
#include "Game/Game.h"

// �R���X�g���N�^
GameplayScene::GameplayScene(Game* pGame)
	: m_pGame{ pGame }
	, m_stage{ this }
	, m_mode{ Mode::Edit }
	, m_ghTileset{}
	, m_cursorEdit{ 0, 0 }
	, m_selectTile{ Tile::TileType::Blick }
	, m_blink{ BLINK_INTEVAL }
{
	// �G�̃��[�h
	m_ghTileset = LoadGraph(L"Resources/Textures/tileset.png");
}

// �f�X�g���N�^
GameplayScene::~GameplayScene()
{
	// �G�̃f�[�^������������폜
	DeleteGraph(m_ghTileset);
}

// ����������
void GameplayScene::Initialize()
{

}

// �X�V����
void GameplayScene::Update(int keyCondition, int keyTrigger)
{
	static int elapseFrame = 0;
	int keyRepeat = 0;

	m_blink.Update();

	if (m_mode == Mode::Edit)
	{
		// �ȈՃL�[���s�[�g
		if (keyCondition)
		{
			elapseFrame++;
			if (elapseFrame >= KEY_REPEAT_INTEVAL)
			{
				elapseFrame = 0;
				keyRepeat = keyCondition;
			}
		}

		// �㕔�J�[�\���̈ړ�
		if (keyRepeat & PAD_INPUT_UP)    m_cursorEdit.y--;
		if (keyRepeat & PAD_INPUT_DOWN)  m_cursorEdit.y++;
		if (keyRepeat & PAD_INPUT_LEFT)  m_cursorEdit.x--;
		if (keyRepeat & PAD_INPUT_RIGHT) m_cursorEdit.x++;

		// �㕔�J�[�\���̉�ʊO�`�F�b�N
		if (m_cursorEdit.x < 0) m_cursorEdit.x = 0;
		if (m_cursorEdit.x > Stage::STAGE_WIDTH - 1) m_cursorEdit.x = Stage::STAGE_WIDTH - 1;
		if (m_cursorEdit.y < 0) m_cursorEdit.y = 0;
		if (m_cursorEdit.y > Stage::STAGE_HEIGHT - 1) m_cursorEdit.y = Stage::STAGE_HEIGHT - 1;

		// Z�L�[�ŃJ�[�\���ʒu�Ɏw��^�C����u��
		if (keyCondition & PAD_INPUT_1)
		{
			m_stage.SetTileType(m_cursorEdit.x, m_cursorEdit.y, m_selectTile);
		}
		// X�L�[�ŃJ�[�\���ʒu�̃^�C��������
		if (keyCondition & PAD_INPUT_2)
		{
			m_stage.SetTileType(m_cursorEdit.x, m_cursorEdit.y, Tile::TileType::None);
		}
		// C�L�[�őI���^�C���̕ύX
		if (keyRepeat & PAD_INPUT_3)
		{
			m_selectTile = static_cast<Tile::TileType>((static_cast<int>(m_selectTile) + 1));
			if (m_selectTile == Tile::TileType::TileTypeMax) m_selectTile = Tile::TileType::Blick;
		}
	}
}

// �`�揈��
void GameplayScene::Render()
{
	// �X�e�[�W�̕`��
	m_stage.Render(m_ghTileset);

	if (m_mode == Mode::Edit)
	{
		int col = static_cast<int>(100.0f + 155.0f * m_blink.GetBlinkRate());
		SetDrawBright(col, col, col);

		// �J�[�\���̕`��
		DrawRectGraph(m_cursorEdit.x * Tile::TILE_WIDTH, m_cursorEdit.y * Tile::TILE_HEIGHT
			, Tile::TILE_WIDTH * 9, Tile::TILE_HEIGHT * 1
			, Tile::TILE_WIDTH, Tile::TILE_HEIGHT, m_ghTileset, TRUE);

		POINT pos[static_cast<int>(Tile::TileType::TileTypeMax)] =
		{
			Tile::TILE_WIDTH * 0, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 0, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 1, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 2, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 3, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 4, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 5, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 6, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 7, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
			Tile::TILE_WIDTH * 8, Stage::STAGE_HEIGHT * Tile::TILE_HEIGHT + 5,
		};
		DrawRectGraph(pos[static_cast<int>(m_selectTile)].x, pos[static_cast<int>(m_selectTile)].y
			, Tile::TILE_WIDTH * 9, Tile::TILE_HEIGHT * 1
			, Tile::TILE_WIDTH, Tile::TILE_HEIGHT, m_ghTileset, TRUE);
		
		SetDrawBright(255, 255, 255);
	}
}

// �I������
void GameplayScene::Finalize()
{
}


