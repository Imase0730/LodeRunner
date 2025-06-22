//--------------------------------------------------------------------------------------
// File: ScoreRankingScene.cpp
//
//--------------------------------------------------------------------------------------
#include "ScoreRankingScene.h"
#include "Game/Game.h"
#include "Game/KeyRepeat.h"

// コンストラクタ
ScoreRankingScene::ScoreRankingScene(Game* pGame)
	: m_pGame{ pGame }
	, m_mode{ Mode::Display }
	, m_stringRenderer{ POINT{ 0, 0 }, "" }
	, m_numberRenderer{ POINT{ 0, 0 }, 2, false }
	, m_initialStringRenderer{ POINT{ 0, 0 }, "" }
	, m_levelNumberRenderer{ POINT{0,0}, 3 }
	, m_scoreNumberRenderer{ POINT{0,0}, 8 }
	, m_entryIndex{ -1 }
	, m_blink{ BLINK_INTERVAL }
	, m_inputCharacter{ 'A' }
	, m_waitTimer{ 0 }
{
}

// デストラクタ
ScoreRankingScene::~ScoreRankingScene()
{
}

// 初期化処理
void ScoreRankingScene::Initialize()
{
	// 表示モード
	m_mode = Mode::Display;

	// 登録するスコアがあるか？
	int score = m_pGame->GetEntryScore().score;
	if (score)
	{
		// 登録する位置を求める 
		int index = GetScoreIndex(score);
		if (index < Game::SCORE_ENTRY_MAX)
		{
			// 登録モード
			m_mode = Mode::Entry;

			// 登録位置
			m_entryIndex = index;

			// 登録する位置を空ける
			for (int i = 0; i < Game::SCORE_ENTRY_MAX - 1 - index; i++)
			{
				int idx = Game::SCORE_ENTRY_MAX - 1 - i;
				m_pGame->SetScore(idx, m_pGame->GetScore(idx - 1));
			}

			// 今回登録するスコアを設定
			m_pGame->SetScore(index, m_pGame->GetEntryScore());

			// 今回登録するスコアを初期化
			m_pGame->InitializeEntryScore();
		}
	}
}

// 更新処理
void ScoreRankingScene::Update(int keyCondition, int keyTrigger)
{
	// 点滅の更新
	m_blink.Update();

	// 画面切り替え時のウエイト
	if (m_waitTimer > 0)
	{
		m_waitTimer--;
		if (m_waitTimer == 0)
		{
			// タイトルへ
			m_pGame->RequestSceneChange(Game::SceneID::Title);
		}
		return;
	}

	// 表示モード
	if (m_mode == Mode::Display)
	{
		// Qキーでタイトルへ
		if (keyTrigger & PAD_INPUT_7)
		{
			m_pGame->RequestSceneChange(Game::SceneID::Title);
		}
	}
	else
	{
		// 登録モード

		// キーリピートを取得
		int keyRepeat = KeyRepeat::GetKey(keyCondition);

		// 左キーで文字選択
		if (keyRepeat & PAD_INPUT_LEFT)
		{
			if (m_inputCharacter != 'A') m_inputCharacter--;
		}

		// 右キーで文字選択
		if (keyRepeat & PAD_INPUT_RIGHT)
		{
			if (m_inputCharacter != '[') m_inputCharacter++;
		}

		// Zキーで決定
		if (keyTrigger & PAD_INPUT_1)
		{
			Game::Score score = m_pGame->GetScore(m_entryIndex);
			score.initial += m_inputCharacter;
			m_inputCharacter = 'A';
			m_pGame->SetScore(m_entryIndex, score);

			// 入力終了？
			if (score.initial.size() == 3)
			{
				// スコアのセーブ
				m_pGame->SaveScore();
				// 画面切り替え時のウエイト値を設定
				m_waitTimer = TRANSITION_DELAY_FRAMES;
			}
		}
	}
}

// 描画処理
void ScoreRankingScene::Render(int ghTileset)
{
	// タイトル
	m_stringRenderer.SetString("LODE RUNNER HIGH SCORES");
	m_stringRenderer.SetPosition(POINT{ 4 * Tile::TILE_PIXEL_WIDTH, 0 });
	m_stringRenderer.Render(ghTileset);

	m_stringRenderer.SetString("INITIALS LEVEL  SCORE");
	m_stringRenderer.SetPosition(POINT{ 4 * Tile::TILE_PIXEL_WIDTH, 3 * Tile::TILE_PIXEL_HEIGHT });
	m_stringRenderer.Render(ghTileset);

	// 横の線
	for (int i = 0; i < 23; i++)
	{
		if ((i != 8) && (i != 14))
		{
			DrawRectGraph(4 * Tile::TILE_PIXEL_WIDTH + i * Tile::TILE_PIXEL_WIDTH, 4 * Tile::TILE_PIXEL_HEIGHT
				, Tile::TILE_PIXEL_WIDTH * 7, Tile::TILE_PIXEL_HEIGHT * 3
				, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT
				, ghTileset, TRUE);
		}
	}

	// スコア
	for (int i = 0; i < Game::SCORE_ENTRY_MAX; i++)
	{
		// 順位
		m_numberRenderer.SetNumber(i + 1);
		m_numberRenderer.SetPosition(POINT{ 0, (5 + i) * Tile::TILE_PIXEL_HEIGHT });
		m_numberRenderer.Render(ghTileset);

		// ドット
		DrawRectGraph(2 * Tile::TILE_PIXEL_WIDTH, (5 + i) * Tile::TILE_PIXEL_HEIGHT
			, Tile::TILE_PIXEL_WIDTH * 6, Tile::TILE_PIXEL_HEIGHT * 3
			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT
			, ghTileset, TRUE);
		
		// スコアが登録されていれば
		if (m_pGame->GetScore(i).level != 0)
		{
			// イニシャル
			m_initialStringRenderer.SetString(m_pGame->GetScore(i).initial.c_str());
			m_initialStringRenderer.SetPosition(POINT{ 7 * Tile::TILE_PIXEL_WIDTH, (5 + i) * Tile::TILE_PIXEL_HEIGHT });
			m_initialStringRenderer.Render(ghTileset);

			// レベル
			m_levelNumberRenderer.SetNumber(m_pGame->GetScore(i).level);
			m_levelNumberRenderer.SetPosition(POINT{ 14 * Tile::TILE_PIXEL_WIDTH, (5 + i) * Tile::TILE_PIXEL_HEIGHT });
			m_levelNumberRenderer.Render(ghTileset);

			// スコア
			m_scoreNumberRenderer.SetNumber(m_pGame->GetScore(i).score);
			m_scoreNumberRenderer.SetPosition(POINT{ 19 * Tile::TILE_PIXEL_WIDTH, (5 + i) * Tile::TILE_PIXEL_HEIGHT });
			m_scoreNumberRenderer.Render(ghTileset);
		}

		// スコアエントリーなら
		if (m_mode == Mode::Entry)
		{
			// 点滅する入力中の文字
			Game::Score score = m_pGame->GetScore(m_entryIndex);
			int size = score.initial.size();
			if ((m_blink.IsBlinkOn()) && (i == m_entryIndex) && (size < 3))
			{
				char str[]{ m_inputCharacter, '\0' };
				m_initialStringRenderer.SetString(str);
				m_initialStringRenderer.SetPosition(POINT{ (7 + size) * Tile::TILE_PIXEL_WIDTH, (5 + i) * Tile::TILE_PIXEL_HEIGHT });
				m_initialStringRenderer.Render(ghTileset);
			}
		}
	}
}

// 終了処理
void ScoreRankingScene::Finalize()
{
}

// 登録する位置を取得する関数
int ScoreRankingScene::GetScoreIndex(int score)
{
	int i{};

	for (i = 0; i < Game::SCORE_ENTRY_MAX; i++)
	{
		if (score > m_pGame->GetScore(i).score) return i;
	}
	
	return i;
}

// デバッグ情報を表示する関数
void ScoreRankingScene::DisplayDebugInformation(int offsetX, int offsetY) const
{
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 0, GetColor(255, 255, 255)
		, L"<< Keys >>");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 2, GetColor(255, 255, 255)
		, L"QUIT   Q");

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 5, GetColor(255, 255, 255)
		, L"<Score Entry>");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 7, GetColor(255, 255, 255)
		, L"←  →");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 9, GetColor(255, 255, 255)
		, L"ENTER   Z");
}

