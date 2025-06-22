//--------------------------------------------------------------------------------------
// File: ScoreRankingScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/StringRenderer.h"
#include "Game/NumberRenderer.h"
#include "Game/Blink.h"

// クラスの前方宣言
class Game;

// ゲームプレイシーン
class ScoreRankingScene
{
	// 列挙型の定義 -----------------------------------------------------
public:

	// モード
	enum class Mode
	{
		Display,	// スコア表示
		Entry,		// スコア登録
	};

// クラス定数の宣言 -------------------------------------------------
private:

	// 点滅の間隔
	static constexpr int BLINK_INTERVAL = 16;

	// 画面切り替え時のウエイト
	static constexpr int TRANSITION_DELAY_FRAMES = 180;

// データメンバの宣言 -----------------------------------------------
private:

	// このシーンを含むゲームオブジェクトへのポインタ
	Game* m_pGame;

	// モード
	Mode m_mode;

	// 文字
	StringRenderer m_stringRenderer;

	// 数字
	NumberRenderer m_numberRenderer;

	// イニシャルの文字
	StringRenderer m_initialStringRenderer;

	// レベルの数字
	NumberRenderer m_levelNumberRenderer;

	// スコアの数字
	NumberRenderer m_scoreNumberRenderer;

	// スコア登録位置
	int m_entryIndex;

	// 点滅
	Blink m_blink;

	// 入力中の文字
	char m_inputCharacter;

	// 終了時の画面切り替えウエイトタイマー
	int m_waitTimer;

// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	ScoreRankingScene(Game* pGame);

	// デストラクタ
	~ScoreRankingScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(int keyCondition, int keyTrigger);

	// 描画処理
	void Render(int ghTileset);

	// 終了処理
	void Finalize();

private:

	// 登録する位置を取得する関数
	int GetScoreIndex(int score);

public:

	// デバッグ情報を表示する関数
	void DisplayDebugInformation(int offsetX, int offsetY) const;

};
