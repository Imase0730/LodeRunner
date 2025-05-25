//--------------------------------------------------------------------------------------
// File: GamePlayScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/Stage.h"
#include "Game/String.h"
#include "Game/Number.h"

// クラスの前方宣言
class Game;

// ゲームプレイシーン
class GamePlayScene
{
	// 列挙型の定義 -----------------------------------------------------
public:

// クラス定数の宣言 -------------------------------------------------
private:
	
	// キーリピートの間隔
	static constexpr int KEY_REPEAT_INTEVAL = 10;

	// 点滅の間隔
	static constexpr int BLINK_INTEVAL = 60;

// データメンバの宣言 -----------------------------------------------
private:

	// このシーンを含むゲームオブジェクトへのポインタ
	Game* m_pGame;

	// ステージ
	Stage m_stage;

	// SCOREの文字列
	String m_scoreString;

	// MENの文字列
	String m_menString;

	// LEVELの文字列
	String m_levelString;

	// スコア表示
	Number m_scoreNumber;

	// 残機数表示
	Number m_menNumber;

	// レベル表示
	Number m_levelNumber;

// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	GamePlayScene(Game* pGame);

	// デストラクタ
	~GamePlayScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(int keyCondition, int keyTrigger);

	// 描画処理
	void Render(int ghTileset);

	// 終了処理
	void Finalize();

private:


public:


};
