//--------------------------------------------------------------------------------------
// File: LevelEditScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/Level.h"
#include "Game/Blink.h"
#include "Game/NumberRenderer.h"
#include "Game/StringRenderer.h"
#include "Game/Tile.h"
#include "YesNoDialog.h"

// クラスの前方宣言
class Game;

// ゲームプレイシーン
class LevelEditScene
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

	// レベルの最大値
	static constexpr int LEVEL_MAX = 999;

	// 選択可能なタイルの数
	static constexpr int SELECT_TILE_MAX = 10;

// データメンバの宣言 -----------------------------------------------
private:

	// このシーンを含むゲームオブジェクトへのポインタ
	Game* m_pGame;

	// 編集中のステージ番号
	int m_levelId;

	// ステージ
	Level m_level;

	// 点滅制御
	Blink m_blink;

	// 画面下部のモード
	Mode m_mode;

	// エディットモード用カーソル位置
	POINT m_cursorEdit;

	// 選択中のタイル
	Tile::Type m_selectTile;

	// SAVEの文字列
	StringRenderer m_saveStringRenderer;

	// LOADの文字列
	StringRenderer m_loadStringRenderer;

	// LEVELの文字列
	StringRenderer m_levelStringRenderer;

	// レベル表示
	NumberRenderer m_levelNumberRenderer;

	// YesNoダイアログ
	YesNoDialog m_yesNoDialog;

// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	LevelEditScene(Game* pGame);

	// デストラクタ
	~LevelEditScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(int keyCondition, int keyTrigger);

	// 描画処理
	void Render(int ghTileset);

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
