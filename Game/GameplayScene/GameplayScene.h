//--------------------------------------------------------------------------------------
// File: GamePlayScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/Level.h"
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
private:

	// 復元するレンガのアニメーションステート
	enum class FillAnimationState
	{
		EMPTY, Fill01, Fill02,
	};

	// クラス定数の宣言 -------------------------------------------------
private:
	
	// プレイヤーの数
	static constexpr int MEN_COUNT = 2;

	// 更新間隔（２フレームに１回更新）
	static constexpr int UPDATE_INTERVAL = 2;

	// ガードの行動フェーズの数
	static constexpr int GUARD_PHASE_COUNT = 3;

	// ガードの１フレームに行動する人数のパターンのリスト
	static constexpr int GUARD_PATTERNS_LIST[GUARD_PHASE_COUNT * (Level::GUARD_MAX + 1)] =
	{
		0, 0, 0,	// ガードの人数（0人）
		0, 1, 1,	// ガードの人数（1人）
		1, 1, 1,	// ガードの人数（2人）
		1, 2, 1,	// ガードの人数（3人）
		1, 2, 2,	// ガードの人数（4人）
		2, 2, 2,	// ガードの人数（5人）
	};

	// 掘ったレンガ情報を記憶するワークの最大数
	static constexpr int DIG_BRICK_MAX = 30;

	// 掘ったレンガの復元するまでのフレーム数
	static constexpr int BRICK_FILL_FRAME = 180;

	// レンガの復元アニメーション用
	static constexpr int BRICK_ANIME_TIME_FILL01 = 20;
	static constexpr int BRICK_ANIME_TIME_FILL02 = 10;

	// 復元するレンガの絵の位置
	static constexpr POINT FILL_BRICK_SPRITES[] =
	{
		{ 0, 4 }, { 6, 10 }, { 7, 10 },
	};

	// 構造体の宣言 -------------------------------------------------
private:

	// 掘ったレンガの情報
	struct DigBrick
	{
		POINT position;	// 位置
		int timer;		// 復元タイマー
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
	Level m_level;

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
	int m_levelId;

	// プレイヤー
	Player m_player;

	// ガード
	Gurad* m_pGurad[Level::GUARD_MAX];

	// ガードの行動人数のパターン
	int m_guradPattern[GUARD_PHASE_COUNT];

	// ガードの行動人数のフェーズ（0～2）
	int m_guradPhase;

	// 行動中のガードの番号
	int m_guradNumber;

	// 掘ったレンガの情報記録用
	DigBrick m_digBrick[DIG_BRICK_MAX];

	// ガードの復活する列
	int m_guardResurrectColmun;

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
	void UpdateGurads();

	// ガードの復活処理
	void ResurrectionGuards();

	// 復元中のレンガの描画処理
	void RenderDigBrick(int ghTileset) const;

	// 掘ったレンガを元に戻す処理
	void RestoreDigBrick();

public:

	// 得点を加算する関数
	void AddScore(int score);

	// プレイヤーを取得する関数
	Player* GetPlayer() { return &m_player; }

	// ガードを取得する関数
	Gurad* GetGurad(int colmun, int row);

	// 指定位置のレンガを復元する
	void SetFillBrick(int x, int y);

	// ガードの復活位置を取得する関数
	POINT GetResurrectPosition(int colmun);

};
