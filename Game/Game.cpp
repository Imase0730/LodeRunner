/**
 * @file   Game.cpp
 *
 * @brief  ゲーム全体に関するソースファイル
 *
 * @author 制作者名
 *
 * @date   日付
 */

// ヘッダファイルの読み込み ===================================================
#include "Game.h"

#include "Game/Screen.h"
#include <fstream>
#include <sstream>



// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param なし
 */
Game::Game()
	: m_key{ 0 }
	, m_oldKey{ 0 }
	, m_currentSceneID{ SceneID::None }
	, m_requestedSceneID{ SceneID::None }
	, m_titleScene{ this }
	, m_gamePlayScene{ this }
	, m_levelEditScene{ this }
	, m_scoreRankingScene{ this }
	, m_ghScreen{ -1 }
	, m_ghTileset{ -1 }
	, m_irisWipe{}
	, m_scores{}
	, m_entryScore{}
	, m_sound{}
{
	// 乱数の初期値を設定
	SRand(static_cast<int>(time(nullptr)));

	// 描画先のグラフィックを作成する
	m_ghScreen = MakeScreen(Screen::GAME_WIDTH, Screen::GAME_HEIGHT, false);
}



/**
 * @brief デストラクタ
 */
Game::~Game()
{

}



/**
 * @brief 初期化処理
 *
 * @param なし
 *
 * @return なし
 */
void Game::Initialize()
{
	// スコアのロード
	LoadScore();

	// 絵のロード
	m_ghTileset = LoadGraph(L"Resources/Textures/tileset.png");

	// スタートシーンの設定
	SetStartScene(SceneID::Title);
}



/**
 * @brief 更新処理
 *
 * @param なし
 *
 * @return なし
 */
void Game::Update(float elapsedTime)
{
	// キー入力の取得
	m_oldKey = m_key;
	m_key    = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// シーンの切り替えリクエストがある？
	if (m_requestedSceneID != SceneID::None)
	{
		// シーンを切り替える
		ChangeScene();
	}

	// 現在のシーンを更新
	UpdateCurrentScene(m_key, ~m_oldKey & m_key);
	
	// ワイプの更新処理
	m_irisWipe.Update();
}



/**
 * @brief 描画処理
 *
 * @param[in] なし
 *
 * @return なし
 */
void Game::Render()
{
	SetBackgroundColor(0, 0, 0);

	// 作成した画像を描画対象にする
	SetDrawScreen(m_ghScreen);

	// 描画対象をクリア
	ClearDrawScreen();

	// ワイプの描画処理
	m_irisWipe.Render();

	// 現在のシーンを描画
	RenderCurrentScene();

	// マスク画面を黒色で塗りつぶす
	FillMaskScreen(0);

	// 描画対象を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// 描画対象画像を拡大して描画する
	int width = static_cast<int>(Screen::GAME_WIDTH * Screen::ZOOM_RATIO);
	int height = static_cast<int>(Screen::GAME_HEIGHT * Screen::ZOOM_RATIO);
	int x = Screen::WIDTH / 2 - width / 2;
	int y = Screen::HEIGHT / 2 - height / 2;
	DrawExtendGraph(x, y, x + width, y + height, m_ghScreen, false);

#if defined(_DEBUG)
	// デバッグ情報の表示
	DisplayDebugInformation(20, 60);
#endif

}



/**
 * @brief 終了処理
 *
 * @param なし
 *
 * @return なし
 */
void Game::Finalize()
{
	// 現在のシーンの終了処理
	FinalizeCurrentScene();
}

// シーン変更の要求
void Game::RequestSceneChange(SceneID nextSceneID)
{
	m_requestedSceneID = nextSceneID;
}

// スコアデータのロード
bool Game::LoadScore()
{
	// ファイルオープン
	std::ifstream ifs(SCORE_DATA_FILENAME);
	if (!ifs)
	{
		return false;
	}

	std::string line;

	for (int i = 0; i < SCORE_ENTRY_MAX; i++)
	{
		if (!std::getline(ifs, line)) break;

		std::stringstream ss(line);
		std::string item;
		Game::Score score;

		// イニシャル
		std::getline(ss, item, ',');
		score.initial = item;

		// レベル
		std::getline(ss, item, ',');
		score.level = std::stoi(item);

		// スコア
		std::getline(ss, item, ',');
		score.score = std::stoi(item);

		// スコア登録
		SetScore(i, score);
	}

	//ファイルを閉じる
	ifs.close();

	return true;
}

// スコアデータのセーブ
bool Game::SaveScore() const
{
	// ファイルオープン
	std::ofstream ofs(SCORE_DATA_FILENAME);
	if (!ofs)
	{
		return false;
	}

	for (int i = 0; i < SCORE_ENTRY_MAX; i++)
	{
		if (!m_scores[i].level) break;
		ofs << m_scores[i].initial << "," << m_scores[i].level << "," << m_scores[i].score << "," << std::endl;
	}

	//ファイルを閉じる
	ofs.close();

	return false;
}

// 登録するスコアを初期化する関数
void Game::InitializeEntryScore()
{
	m_entryScore.score = 0;
}

// 登録するスコアを設定する関数
void Game::SetEntryScore(Score score)
{
	m_entryScore = score;
}

// 登録するスコアを取得する関数
Game::Score Game::GetEntryScore() const
{
	return m_entryScore;
}

// 開始シーンの設定
void Game::SetStartScene(SceneID startSceneID)
{
	m_currentSceneID = startSceneID;
	InitializeCurrentScene();
}

// シーンの変更
void Game::ChangeScene()
{
	// 現在のシーンを終了させる
	FinalizeCurrentScene();
	// シーンIDの変更
	m_currentSceneID = m_requestedSceneID;
	// 新しいシーンを初期化する
	InitializeCurrentScene();
	// シーンの変更が終わったので変更要求をリセット
	m_requestedSceneID = SceneID::None;
}

// 現在のシーンの初期化処理
void Game::InitializeCurrentScene()
{
	switch (m_currentSceneID)
	{
	case SceneID::Title:		// タイトル
		m_titleScene.Initialize();
		break;
	case SceneID::GamePlay:		// ゲームプレイ
		m_gamePlayScene.Initialize();
		break;
	case SceneID::LevelEdit:	// ステージエディット
		m_levelEditScene.Initialize();
		break;
	case SceneID::ScoreRanking:	// スコアランキング
		m_scoreRankingScene.Initialize();
		break;
	default:
		break;
	}
}

// 現在のシーンの更新処理
void Game::UpdateCurrentScene(int keyCondition, int keyTrigger)
{
	switch (m_currentSceneID)
	{
	case SceneID::Title:		// タイトル
		m_titleScene.Update(keyCondition, keyTrigger);
		break;
	case SceneID::GamePlay:		// ゲームプレイ
		m_gamePlayScene.Update(keyCondition, keyTrigger);
		break;
	case SceneID::LevelEdit:	// ステージエディット
		m_levelEditScene.Update(keyCondition, keyTrigger);
		break;
	case SceneID::ScoreRanking:	// スコアランキング
		m_scoreRankingScene.Update(keyCondition, keyTrigger);
		break;
	default:
		break;
	}
}

// 現在のシーンの描画処理
void Game::RenderCurrentScene()
{
	switch (m_currentSceneID)
	{
	case SceneID::Title:		// タイトル
		m_titleScene.Render(m_ghTileset);
		break;
	case SceneID::GamePlay:		// ゲームプレイ
		m_gamePlayScene.Render(m_ghTileset);
		break;
	case SceneID::LevelEdit:	// ステージエディット
		m_levelEditScene.Render(m_ghTileset);
		break;
	case SceneID::ScoreRanking:	// スコアランキング
		m_scoreRankingScene.Render(m_ghTileset);
		break;
	default:
		break;
	}

}

// 現在のシーンの終了処理
void Game::FinalizeCurrentScene()
{
	switch (m_currentSceneID)
	{
	case SceneID::Title:		// タイトル
		m_titleScene.Finalize();
		break;
	case SceneID::GamePlay:		// ゲームプレイ
		m_gamePlayScene.Finalize();
		break;
	case SceneID::LevelEdit:	// ステージエディット
		m_levelEditScene.Finalize();
		break;
	case SceneID::ScoreRanking:	// スコアランキング
		m_scoreRankingScene.Finalize();
		break;
	default:
		break;
	}
}

// デバッグ情報を表示する関数
void Game::DisplayDebugInformation(int offsetX, int offsetY) const
{
	switch (m_currentSceneID)
	{
	case SceneID::Title:		// タイトル
		m_titleScene.DisplayDebugInformation(offsetX, offsetY);
		break;
	case SceneID::GamePlay:		// ゲームプレイ
		m_gamePlayScene.DisplayDebugInformation(offsetX, offsetY);
		break;
	case SceneID::LevelEdit:	// ステージエディット
		m_levelEditScene.DisplayDebugInformation(offsetX, offsetY);
		break;
	case SceneID::ScoreRanking:	// スコアランキング
		m_scoreRankingScene.DisplayDebugInformation(offsetX, offsetY);
		break;
	default:
		break;
	}
}

