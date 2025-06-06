//--------------------------------------------------------------------------------------
// File: GamePlayScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/Stage.h"
#include "Game/String.h"
#include "Game/Number.h"
#include "Player.h"

// クラスの前方宣言
class Game;
class IrisWipe;

// ゲームプレイシーン
class GamePlayScene
{
	// 列挙型の定義 -----------------------------------------------------
public:

// クラス定数の宣言 -------------------------------------------------
private:
	
	// プレイヤーの数
	static constexpr int MEN_COUNT = 2;

	// 更新間隔（２フレームに１回更新）
	static constexpr int UPDATE_INTERVAL = 2;

// データメンバの宣言 -----------------------------------------------
private:

	// このシーンを含むゲームオブジェクトへのポインタ
	Game* m_pGame;

	// 更新カウンター
	int m_updateCounter;

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

	// スコア
	int m_score;

	// 残機数
	int m_men;

	// レベル
	int m_level;

	// プレイヤー
	Player m_player;

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

	// ゲームスタート時の初期化
	void GameInitialize();

};
