//--------------------------------------------------------------------------------------
// File: HighScoresScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/StringRenderer.h"
#include "Game/NumberRenderer.h"

// クラスの前方宣言
class Game;

// ゲームプレイシーン
class HighScoresScene
{
	// 列挙型の定義 -----------------------------------------------------
public:

	// モード
	enum class Mode
	{
		Game,		// ゲーム
		Edit,		// エディット
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

// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	HighScoresScene(Game* pGame);

	// デストラクタ
	~HighScoresScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(int keyCondition, int keyTrigger);

	// 描画処理
	void Render(int ghTileset);

	// 終了処理
	void Finalize();

};
