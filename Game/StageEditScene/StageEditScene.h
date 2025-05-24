//--------------------------------------------------------------------------------------
// File: StageEditScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/Stage.h"
#include "Game/Blink.h"
#include "Game/Number.h"
#include "Game/String.h"

// クラスの前方宣言
class Game;

// ゲームプレイシーン
class StageEditScene
{
	// 列挙型の定義 -----------------------------------------------------
public:

	// 画面下部のモード
	enum class Mode
	{
		SelectTile,	// タイル選択
		Save,		// セーブ
		Load,		// ロード
	};

// クラス定数の宣言 -------------------------------------------------
private:
	
	// キーリピートの間隔
	static constexpr int KEY_REPEAT_INTEVAL = 10;

	// 点滅の間隔
	static constexpr int BLINK_INTEVAL = 60;

	// ステージの最大値
	static constexpr int STAGE_MAX = 999;

// データメンバの宣言 -----------------------------------------------
private:

	// このシーンを含むゲームオブジェクトへのポインタ
	Game* m_pGame;

	// レベル
	int m_level;

	// ステージ
	Stage m_stage;

	// グラフィックハンドル
	int m_ghTileset;

	// 点滅制御
	Blink m_blink;

	// 画面下部のモード
	Mode m_mode;

	// エディットモード用カーソル位置
	POINT m_cursorEdit;

	// 選択中のタイル
	Tile::TileType m_selectTile;

	// SAVEの文字列
	String m_saveString;

	// LOADの文字列
	String m_loadString;

	// LEVELの文字列
	String m_levelString;

	// レベル表示
	Number m_levelNumber;

// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	StageEditScene(Game* pGame);

	// デストラクタ
	~StageEditScene();

	// 初期化処理
	void Initialize(int ghTileset);

	// 更新処理
	void Update(int keyCondition, int keyTrigger);

	// 描画処理
	void Render();

	// 終了処理
	void Finalize();

private:

	// 簡易キーリピート
	int KeyRepeat(int keyCondition);

	// タイル選択
	void SelectTile(int keyCondition, int keyRepeat);

	// セーブ
	void Save(int keyTrigger, int keyRepeat);

	// ロード
	void Load(int keyTrigger, int keyRepeat);

};
