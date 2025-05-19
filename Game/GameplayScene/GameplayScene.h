//--------------------------------------------------------------------------------------
// File: GameplayScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Stage.h"

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
	
	static constexpr int REPEAT_INTEVAL = 10;

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
