//--------------------------------------------------------------------------------------
// File: GameplayScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Stage.h"
#include "Blink.h"

// クラスの前方宣言
class Game;

// ゲームプレイシーン
class GameplayScene
{
	// 列挙型の定義 -----------------------------------------------------
public:

	// モード
	enum class Mode
	{
		GamePlay,
		Edit,
	};

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

	// モード
	Mode m_mode;

	// グラフィックハンドル
	int m_ghTileset;

	// 点滅制御
	Blink m_blink;

// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	GameplayScene(Game* pGame);

	// デストラクタ
	~GameplayScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(int keyCondition, int keyTrigger);

	// 描画処理
	void Render();

	// 終了処理
	void Finalize();

private:

	// エディットモード用カーソル位置
	POINT m_cursorEdit;

	// 選択中のタイル
	Tile::TileType m_selectTile;

public:

	// モードの取得関数
	Mode GetMode() const { return m_mode; }

};
