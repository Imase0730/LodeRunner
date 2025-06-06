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
#include "Gurad.h"

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

	// ガードの１フレームに行動する人数のパターンのリスト
	static constexpr int GUARD_PATTERNS_LIST[] =
	{
		0, 0, 0,	// ガードの人数（0人）
		0, 1, 1,	// ガードの人数（1人）
		1, 1, 1,	// ガードの人数（2人）
		1, 2, 1,	// ガードの人数（3人）
		1, 2, 2,	// ガードの人数（4人）
		2, 2, 2,	// ガードの人数（5人）
	};

public:

	// 金塊の得点
	static constexpr int GOLD_SCORE = 250;

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

	// ガード
	Gurad* m_pGurad[Stage::GUARD_MAX];

	// ガードの行動人数のパターン
	int m_guradPattern[3];

	// ガードの行動人数のフェーズ（0〜2）
	int m_guradPhase;

	// 行動中のガードの番号
	int m_guradNumber;

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

	// レベルクリアか調べる関数
	bool IsLevelCleared();

	// ガードの更新処理
	void GuradsUpdate();

public:

	// 得点を加算する関数
	void AddScore(int score);

};
