//--------------------------------------------------------------------------------------
// File: GamePlayScene.cpp
//
//--------------------------------------------------------------------------------------
#include "GamePlayScene.h"
#include "Game/Game.h"
#include "Game/Tile.h"
#include <fstream>
#include <sstream>

// �R���X�g���N�^
GamePlayScene::GamePlayScene(Game* pGame)
	: m_pGame{ pGame }
	, m_updateCounter{ 0 }
	, m_level{ Level::Mode::GamePlay }
	, m_scoreStringRenderer{ POINT{ Tile::TILE_PIXEL_WIDTH * 0 , Level::INFOMATION_Y }, "SCORE" }
	, m_menStringRenderer{ POINT{ Tile::TILE_PIXEL_WIDTH * 13 , Level::INFOMATION_Y }, "MEN" }
	, m_levelStringRenderer{ POINT{ Tile::TILE_PIXEL_WIDTH * 20 , Level::INFOMATION_Y }, "LEVEL" }
	, m_scoreNumberRenderer{ POINT{ 5 * Tile::TILE_PIXEL_WIDTH, Level::INFOMATION_Y }, 7 }
	, m_menNumberRenderer{ POINT{ 16 * Tile::TILE_PIXEL_WIDTH, Level::INFOMATION_Y }, 3 }
	, m_levelNumberRenderer{ POINT{ 25 * Tile::TILE_PIXEL_WIDTH, Level::INFOMATION_Y }, 3 }
	, m_score{ 0 }
	, m_men{ 0 }
	, m_levelId{ 0 }
	, m_player{ this, &m_level }
	, m_pGuard{}
	, m_GuardPattern{ 0, 0, 0 }
	, m_GuardPhase{ 0 }
	, m_GuardNumber{ 0 }
	, m_digBrick{}
	, m_guardResurrectColmun{ 0 }
	, m_startWaitTimer{ 0 }
	, m_clearWaitTimer{ 0 }
	, m_levelClearScore{ 0 }
	, m_playerDeadWaitTimer{ 0 }
	, m_isTestPlay{ false }
	, m_yesNoDialog{}
	, m_dialogType{ DialogType::None }
	, m_saveScore{ 0 }
{
	// �K�[�h�𐶐�
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		m_pGuard[i] = new Guard(this, &m_level);
	}
}

// �f�X�g���N�^
GamePlayScene::~GamePlayScene()
{
	// �����������
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		delete m_pGuard[i];
	}
}

// ����������
void GamePlayScene::Initialize()
{
	// �e�X�g�v���C���ł͂Ȃ�
	m_isTestPlay = false;

	// �o�^����X�R�A�̏�����
	m_pGame->InitializeEntryScore();

	// �X�R�A�̏�����
	m_score = m_saveScore = 0;
	m_scoreNumberRenderer.SetNumber(m_score);

	// �v���C���[�̐��̏�����
	m_men = MEN_COUNT;
	m_menNumberRenderer.SetNumber(m_men);

	// ���x���̏�����
	m_levelId = 1;
	m_levelNumberRenderer.SetNumber(m_levelId);

	// �Q�[���X�^�[�g���̏�����
	GameInitialize();

	// ���C�v�I�[�v��
	m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Open);
}

// �X�V����
void GamePlayScene::Update(int keyCondition, int keyTrigger)
{
	// �X�V�Ԋu�𒲐�����
	if (++m_updateCounter < UPDATE_INTERVAL) return;
	m_updateCounter = 0;

	static int oldKey = 0;

	keyTrigger = ~oldKey & keyCondition;

	// ���̃��x���ւ̈ڍs����
	if (WipeToNextLevel()) return;

	// �v���C���[���S���̑҂����Ԃ̏���
	if (WaitPlayerDead()) return;

	// ���x���N���A���̑҂����Ԃ̏���
	if (WaitLevelClear()) return;

	// Yes/No�_�C�A���O���\�����̏���
	if (HandleDialogState(keyTrigger))
	{
		oldKey = keyCondition;
		return;
	}

	// Q�L�[�ŃQ�[�������g���C
	if (keyTrigger & PAD_INPUT_7)
	{
		m_dialogType = DialogType::Retry;
		m_yesNoDialog.StartDialog("RETRY");
	}

	// F1�L�[�ŃZ�[�u
	if (CheckHitKey(KEY_INPUT_F1))
	{
		m_dialogType = DialogType::Save;
		m_yesNoDialog.StartDialog("SAVE");
	}

	// F2�L�[�Ń��[�h
	if (CheckHitKey(KEY_INPUT_F2))
	{
		m_dialogType = DialogType::Load;
		m_yesNoDialog.StartDialog("LOAD");
	}

	// �X�e�[�W�̍X�V
	m_level.Update();

	// �v���C���[�̍X�V
	m_player.Update(keyCondition, keyTrigger);

	// �K�[�h�̍X�V
	UpdateGuards();

	// ----- �^�C�}�[�֌W�̏��� ----- //

	// �K�[�h�̕�������
	ResurrectionGuards();

	// �K�[�h�̕����ʒu�̍X�V�i����P�����j
	if (++m_guardResurrectColmun > Level::MAX_GAME_COLMUN) m_guardResurrectColmun = 0;

	// �@�����u���b�N�����ɖ߂�����
	RestoreDigBrick();

	// �v���C���[�������Ă���Ȃ�
	if (m_player.IsAlive())
	{
		// ���x���N���A�Ȃ瓾�_���P�T�O�O�_���Z����
		if (IsLevelCleared())
		{
			// �e�X�g�v���C��
			if (m_isTestPlay)
			{
				// ���x���G�f�B�^��
				m_pGame->RequestSceneChange(Game::SceneID::LevelEdit);
			}

			// ���x���N���A���̑҂����Ԃ�ݒ�
			m_clearWaitTimer = CLEAR_WAIT_FRAME;
			// ���x���N���A���̉��Z����X�R�A��ݒ�
			m_levelClearScore = LEVEL_CLEAR_SCORE;
			// ���ʉ���炷�i�N���A���j
			GetSound().PlaySound(Sound::SoundID::Clear);
		}
	}
	else
	{
		// �e�X�g�v���C��
		if (m_isTestPlay)
		{
			// ���x���G�f�B�^��
			m_pGame->RequestSceneChange(Game::SceneID::LevelEdit);
		}

		// �c�@�������炷
		m_menNumberRenderer.SetNumber(--m_men);
		// �v���C���[���S���̑҂����Ԃ�ݒ�
		m_playerDeadWaitTimer = PLAYER_DEAD_WAIT_FRAME;
	}

	oldKey = keyCondition;
}

// �`�揈��
void GamePlayScene::Render(int ghTileset)
{
	// �X�e�[�W�̕`��
	m_level.Render(ghTileset);

	// �������̃����K�̕`�揈��
	RenderDigBrick(ghTileset);

	// �v���C���[�̕`��
	m_player.Render(ghTileset);

	// �K�[�h�̕`��
	for (int i = 0; i < m_level.GetGuardCount(); i++)
	{
		m_pGuard[i]->Render(ghTileset);
	}

	// �wSCORE�x�̕����̕\��
	m_scoreStringRenderer.Render(ghTileset);

	// �wMEN�x�̕����̕\��
	m_menStringRenderer.Render(ghTileset);

	// �wLEVEL�x�̕����̕\��
	m_levelStringRenderer.Render(ghTileset);

	// �X�R�A�̕\��
	m_scoreNumberRenderer.Render(ghTileset);

	// �c�@���̕\��
	m_menNumberRenderer.Render(ghTileset);

	// ���x���̕\��
	m_levelNumberRenderer.Render(ghTileset);

	// Yes/No�_�C�A���O�̕`��
	m_yesNoDialog.Render(ghTileset);
}

// �I������
void GamePlayScene::Finalize()
{
}

// �Q�[���X�^�[�g���̏�����
void GamePlayScene::GameInitialize()
{
	// �Q�[���X�^�[���̑҂����Ԃ�ݒ�
	m_startWaitTimer = START_WAIT_FRAME;

	// �Q�[���N���A���̑҂����Ԃ�������
	m_clearWaitTimer = 0;

	// �v���C���[���S���̑҂����Ԃ�������
	m_playerDeadWaitTimer = 0;

	// �Z�[�u�p�X�R�A�̍X�V
	m_saveScore = m_score;

	// �v���C���[�������Ă���
	if (m_player.IsAlive())
	{
		// �c�@���𑝂₷
		m_men++;

		// ���̃��x����
		m_levelId++;
	}

	// ������̍X�V
	m_scoreNumberRenderer.SetNumber(m_score);
	m_menNumberRenderer.SetNumber(m_men);
	m_levelNumberRenderer.SetNumber(m_levelId);

	// �e�X�g�v���C��
	if (m_isTestPlay)
	{
		// �e�X�g�v���C�p
		Tile::Type (*type)[Level::MAX_GAME_ROW + 1][Level::MAX_GAME_COLMUN + 1] = m_pGame->GetTestPlayData();
		m_level.SetLoadData(*type);
	}
	else
	{
		// ���x���f�[�^�̓ǂݍ���
		if(!m_level.LoadLevel(m_levelId))
		{
			// ���x���f�[�^�ǂݍ��݃G���[�̂��߃X�e�[�W�P�ɖ߂�
			m_levelId = 1;
			m_levelNumberRenderer.SetNumber(m_levelId);
			m_level.LoadLevel(m_levelId);
		}
	}

	// ���x���̏�����
	m_level.Initialize(Level::Mode::GamePlay);

	// �@���������K�����̏�����
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		m_digBrick[i].position = POINT{ 0, 0 };
		m_digBrick[i].timer = 0;
	}

	// �v���C���[�̏�����
	POINT pos = m_level.GetPlayerPosition();
	m_player.Initialize(POINT{ pos.x, pos.y }
	, POINT{ Tile::TILE_CENTER_X, Tile::TILE_CENTER_Y });

	// ----- �K�[�h�̏����� ----- //

	// �K�[�h�̐l���ɉ����čs���\�l���̃e�[�u����ݒ肷��
	for (int i = 0; i < GUARD_PHASE_COUNT; i++)
	{
		m_GuardPattern[i] = GUARD_PATTERNS_LIST[m_level.GetGuardCount() * GUARD_PHASE_COUNT + i];
	}

	// �K�[�h�̈ʒu��ݒ�
	for (int i = 0; i < m_level.GetGuardCount(); i++)
	{
		POINT pos = m_level.GetGuardPosition(i);
		m_pGuard[i]->Initialize(POINT{ pos.x, pos.y }
		, POINT{ Tile::TILE_CENTER_X, Tile::TILE_CENTER_Y });
	}
}

// ���x���N���A�����ׂ�֐�
bool GamePlayScene::IsLevelCleared()
{
	if ( (m_level.GetGoldCount() < 0)								// �B���n�V�S�o���ς�
	  && (m_player.GetTilePosition().y == 0)						// ��ԏ�̍s�H
	  && (m_player.GetAdjustPosition().y == Tile::TILE_CENTER_Y)	// �^�C����̈ʒu�����S�H
	   )
	{
		return true;
	}
	return false;
}

// �K�[�h�̍X�V����
void GamePlayScene::UpdateGuards()
{
	// �P�t���[���ɍs���ł���K�[�h�̐l�������[�v
	for (int i = 0; i < m_GuardPattern[m_GuardPhase]; i++)
	{
		// �K�[�h���A�N�e�B�u�Ȃ�X�V
		if (m_pGuard[m_GuardNumber]->IsActive()) m_pGuard[m_GuardNumber]->Update();
		// ���̍s������K�[�h�ԍ���
		m_GuardNumber = (m_GuardNumber + 1) % m_level.GetGuardCount();
	}
	// ���̍s���\�l����
	m_GuardPhase = (m_GuardPhase + 1) % GUARD_PHASE_COUNT;
}

// �K�[�h�̕�������
void GamePlayScene::ResurrectionGuards()
{
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		int timer = m_pGuard[i]->GetResurrectionTimer();
		if (timer > 0)
		{
			//�X�e�[�g�ɑ傫�Ȓl�����ē����Ȃ��悤�ɂ���
			m_pGuard[i]->SetActionStateTimer(0x7f);
			timer--;
			if (timer == 0)
			{
				// �����ꏊ�ɃK�[�h�����Ȃ���Ε�������
				POINT pos = m_pGuard[i]->GetTilePosition();
				if (m_level.GetTilePage1(pos.x, pos.y) == Tile::Type::Empty)
				{
					m_level.SetTilePage1(pos.x, pos.y, Tile::Type::Empty);
					m_pGuard[i]->Initialize(m_pGuard[i]->GetTilePosition(), m_pGuard[i]->GetAdjustPosition());
					// ���ʉ���炷�i�������鉹�j
					GetSound().PlaySound(Sound::SoundID::Resurrection);
				}
			}
			else
			{
				// �����^�C�}�[���Z
				m_pGuard[i]->SetResurrectionTimer(timer);
			}
		}
	}
}

// �������̃����K�̕`�揈��
void GamePlayScene::RenderDigBrick(int ghTileset) const
{
	// �������̃����K�̕`��
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		// �����^�C�}�[���O�łȂ�
		if (m_digBrick[i].timer)
		{
			// �@���Ă��郌���K�̊G�̈ʒu
			POINT pos = m_digBrick[i].position;
			if (m_digBrick[i].timer <= BRICK_ANIME_TIME_FILL02)
			{
				// �������̃����K�Q
				POINT spritePos = FILL_BRICK_SPRITES[static_cast<int>(FillAnimationState::Fill02)];
				DrawRectGraph(pos.x * Tile::TILE_PIXEL_WIDTH, pos.y * Tile::TILE_PIXEL_HEIGHT
					, Tile::TILE_PIXEL_WIDTH * spritePos.x, Tile::TILE_PIXEL_HEIGHT * spritePos.y
					, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, FALSE);
			}
			else if (m_digBrick[i].timer <= BRICK_ANIME_TIME_FILL01)
			{
				// �������̃����K�P
				POINT spritePos = FILL_BRICK_SPRITES[static_cast<int>(FillAnimationState::Fill01)];
				DrawRectGraph(pos.x * Tile::TILE_PIXEL_WIDTH, pos.y * Tile::TILE_PIXEL_HEIGHT
					, Tile::TILE_PIXEL_WIDTH * spritePos.x, Tile::TILE_PIXEL_HEIGHT * spritePos.y
					, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, FALSE);
			}
			else
			{
				// ��
				POINT spritePos = FILL_BRICK_SPRITES[static_cast<int>(FillAnimationState::EMPTY)];
				DrawRectGraph(pos.x * Tile::TILE_PIXEL_WIDTH, pos.y * Tile::TILE_PIXEL_HEIGHT
					, Tile::TILE_PIXEL_WIDTH * spritePos.x, Tile::TILE_PIXEL_HEIGHT * spritePos.y
					, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, FALSE);
			}
		}
	}
}

// �@���������K�����ɖ߂�����
void GamePlayScene::RestoreDigBrick()
{
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		// �����^�C�}�[���O�łȂ�
		if (m_digBrick[i].timer != 0)
		{
			// �����^�C�}�[�����Z����
			m_digBrick[i].timer--;

			// �����K�ɖ߂�
			if (m_digBrick[i].timer == 0)
			{
				Tile::Type page1 = m_level.GetTilePage1(m_digBrick[i].position.x, m_digBrick[i].position.y);

				// �v���C���[�Ȃ�
				if (page1 == Tile::Type::Player)
				{
					// �v���C���[���S
					m_player.SetAlive(false);
					// �v���[���[���\��
					m_player.SetVisible(false);
				}

				// �K�[�h�Ȃ�
				if (page1 == Tile::Type::Guard)
				{
					// ���񂾃K�[�h��������
					Guard* Guard = GetGuard(m_digBrick[i].position.x, m_digBrick[i].position.y);
					// ���񂾃K�[�h������������Ă�����
					if (Guard->GetActionStateTimer())
					{
						// ����̐������炷

						//�X�e�[�g�ɑ傫�Ȓl�����ē����Ȃ��悤�ɂ���
						Guard->SetActionStateTimer(0x7f);
					}
					// �����ʒu���擾
					POINT pos = GetResurrectPosition(m_guardResurrectColmun);

					// �K�[�h�̕����ݒ�
					Guard->Resurrection(pos.x, pos.y);

					// ���_�����Z�i�V�T�_�j
					AddScore(GamePlayScene::GUARD_KILL_SCORE);
				}

				// ����Ȃ�
				if (page1 == Tile::Type::Gold)
				{
					// ����̐������炷
					m_level.LostGold();
				}

				// �����K�ɖ߂�
				m_level.SetTilePage1(m_digBrick[i].position.x, m_digBrick[i].position.y, Tile::Type::Blick);
			}
		}
	}
}

// ���̃��x���ւ̈ڍs����
bool GamePlayScene::WipeToNextLevel()
{
	// ���C�v���쒆�Ȃ牽�����Ȃ�
	if (m_pGame->GetIrisWipe()->IsActive()) return true;

	// ���C�v��������
	if (m_pGame->GetIrisWipe()->GetMode() == IrisWipe::Mode::Close)
	{
		// �Q�[����������
		GameInitialize();

		// ���C�v�I�[�v��
		m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Open);

		return true;
	}

	// ���C�v���I�������琔�t���[���v���C���[��_�ł����Ă���Q�[�����J�n����
	if ( (m_pGame->GetIrisWipe()->GetMode() == IrisWipe::Mode::None)
	  && (m_startWaitTimer != 0)
	   )
	{
		// �Q�[���X�^�[�g���̃E�G�C�g
		if (m_startWaitTimer > 0) m_startWaitTimer--;
		if (m_startWaitTimer != 0)
		{
			// �v���C���[��_��
			if ((m_startWaitTimer / 4) % 2)
			{
				m_player.SetVisible(true);
			}
			else
			{
				m_player.SetVisible(false);
			}
			return true;
		}
		else
		{
			// �_�ŏI����A�v���C���[��\��(ON)
			m_player.SetVisible(true);
		}
	}

	return false;
}

// ���x���N���A���̑҂����Ԃ̏���
bool GamePlayScene::WaitLevelClear()
{
	if (m_clearWaitTimer != 0)
	{
		if (m_clearWaitTimer > 0) m_clearWaitTimer--;
		// ���̃��x���ֈڍs���邽�߃��C�v���N���[�Y����
		if (m_clearWaitTimer == 0)
		{
			m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Close);
		}
		// 100�_���������i���v1500�_�j
		int score = 100;
		m_levelClearScore -= score;
		if (m_levelClearScore >= 0)
		{
			AddScore(score);
		}
		return true;
	}
	return false;
}

// �v���C���[���S���̑҂����Ԃ̏���
bool GamePlayScene::WaitPlayerDead()
{
	// ��ʐ؂�ւ����̃E�G�C�g
	if (m_playerDeadWaitTimer > 0)
	{
		m_playerDeadWaitTimer--;
		if (m_playerDeadWaitTimer == 0)
		{
			// �c�@�����O�Ȃ�
			if (m_men == 0)
			{
				// �X�R�A�o�^�\�H
				if (m_score > m_pGame->GetScore(Game::SCORE_ENTRY_MAX - 1).score)
				{
					// �X�R�A�o�^��
					Game::Score score{ "", m_levelId, m_score };
					m_pGame->SetEntryScore(score);
					m_pGame->RequestSceneChange(Game::SceneID::ScoreRanking);
				}
				else
				{
					// �^�C�g����
					m_pGame->RequestSceneChange(Game::SceneID::Title);
				}
			}
			else
			{
				// ���C�v�N���[�Y
				m_pGame->GetIrisWipe()->Start(IrisWipe::Mode::Close);
			}
		}
		return true;
	}
	return false;
}

// �T�E���h���擾����֐�
const Sound& GamePlayScene::GetSound()
{
	return m_pGame->GetSound();
}

// ���_�����Z����֐�
void GamePlayScene::AddScore(int score)
{
	m_score += score;
	m_scoreNumberRenderer.SetNumber(m_score);
}

// �K�[�h���擾����֐�
Guard* GamePlayScene::GetGuard(int colmun, int row)
{
	for (int i = 0; i < Level::GUARD_MAX; i++)
	{
		// �w��ʒu�ɂ���K�[�h�ւ̃|�C���^��Ԃ�
		POINT pos = m_pGuard[i]->GetTilePosition();
		if ((pos.x == colmun) && (pos.y == row))
		{
			return m_pGuard[i];
		}
	}
	return nullptr;
}

// �w��ʒu�̃����K�𕜌�����
void GamePlayScene::SetFillBrick(int x, int y)
{
	for (int i = 0; i < DIG_BRICK_MAX; i++)
	{
		// ���g�p�̃��[�N�Ȃ�
		if (m_digBrick[i].timer == 0)
		{
			// �����K�̕�������ݒ�
			m_digBrick[i].position.x = x;
			m_digBrick[i].position.y = y;
			m_digBrick[i].timer = BRICK_FILL_FRAME;
			return;
		}
	}
}

// �K�[�h�̕����ʒu���擾����֐�
POINT GamePlayScene::GetResurrectPosition(int colmun)
{
	int row = 1;

	// ��̍s���珇�Ԃɋ󂢂Ă���ʒu��T��
	while (row <= Level::MAX_GAME_ROW)
	{
		for (int i = colmun; i < Level::MAX_GAME_COLMUN; i++)
		{
			// �󔒂ȏꏊ��T��
			if ( (m_level.GetTilePage2(i, row) == Tile::Type::Empty)
			  && (m_level.GetTilePage1(i, row) == Tile::Type::Empty)
			   )
			{
				return POINT{ i, row };
			}
		}
		row++;
		colmun = 0;
	}

	return POINT{ -1, -1 };
}

// �f�o�b�O����\������֐�
void GamePlayScene::DisplayDebugInformation(int offsetX, int offsetY) const
{
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 0, GetColor(255, 255, 255)
		, L"<< Keys >>");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 2, GetColor(255, 255, 255)
		, L"       ��");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 3, GetColor(255, 255, 255)
		, L"MOVE ���@��");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 4, GetColor(255, 255, 255)
		, L"       ��");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 6, GetColor(255, 255, 255)
		, L"DIG   Z  X");
	
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 8, GetColor(255, 255, 255)
		, L"RETRY  Q");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 10, GetColor(255, 255, 255)
		, L"SAVE  F1");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 12, GetColor(255, 255, 255)
		, L"LOAD  F2");

}

// �e�X�g�v���C���̏�����
void GamePlayScene::InitializeTestPlay()
{
	// �e�X�g�v���C��
	m_isTestPlay = true;

	// �X�R�A�̏�����
	m_score = 0;
	m_scoreNumberRenderer.SetNumber(m_score);

	// �v���C���[�̐��̏�����
	m_men = 1;
	m_menNumberRenderer.SetNumber(m_men);

	// ���x���̏�����
	m_levelId = m_pGame->GetTestPlayLevel();
	m_levelNumberRenderer.SetNumber(m_levelId);

	// �Q�[���X�^�[�g���̏�����
	GameInitialize();
}

// �Z�[�u
bool GamePlayScene::Save()
{
	std::ofstream ofs(SAVE_DATA_FILENAME);

	if (!ofs)
	{
		return false;
	}

	// ���x���A�c�@���A�X�R�A
	ofs << m_levelId << "," << m_men << "," << m_saveScore << "," << std::endl;

	//�t�@�C�������
	ofs.close();

	return true;
}

// ���[�h
bool GamePlayScene::Load()
{
	std::ifstream ifs(SAVE_DATA_FILENAME);

	if (!ifs)
	{
		return false;
	}

	// ���x���A�c�@���A�X�R�A�̓ǂݍ���
	std::string line;
	getline(ifs, line);
	std::istringstream iss(line);
	std::string item;
	getline(iss, item, ',');
	m_levelId = std::stoi(item);
	getline(iss, item, ',');
	m_men = std::stoi(item);
	getline(iss, item, ',');
	m_saveScore = std::stoi(item);

	//�t�@�C�������
	ifs.close();

	return true;
}

// �_�C�A���O���\������Ă��鎞�̏���
bool GamePlayScene::HandleDialogState(int keyTrigger)
{
	// YesNo�_�C�A���O�\�����H
	if (m_yesNoDialog.IsVisivle())
	{
		// Yes���I�����ꂽ�H
		if ((m_yesNoDialog.Update(keyTrigger) == true)
			&& (m_yesNoDialog.GetYesNo() == YesNoDialog::YesNo::Yes)
			)
		{
			switch (m_dialogType)
			{
			case DialogType::Load:
				if (!Load()) break;
			case DialogType::Retry:
				// �v���C���[���S���̑҂����Ԃ�ݒ�
				m_playerDeadWaitTimer = PLAYER_DEAD_WAIT_FRAME;
				// �X�R�A��߂�
				m_score = m_saveScore;
				// �v���C���[���E��
				m_player.SetAlive(false);
				break;
			case DialogType::Save:
				Save();
				break;
			default:
				break;
			}
		}
		return true;
	}
	return false;
}

