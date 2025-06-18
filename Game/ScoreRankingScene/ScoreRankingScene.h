//--------------------------------------------------------------------------------------
// File: ScoreRankingScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/StringRenderer.h"
#include "Game/NumberRenderer.h"

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
	
	// 入力中の文字
	char m_character;

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

};
