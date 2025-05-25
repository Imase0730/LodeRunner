//--------------------------------------------------------------------------------------
// File: TitleScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/String.h"
#include "Game/Tile.h"
#include "Game/Blink.h"

// クラスの前方宣言
class Game;

// ゲームプレイシーン
class TitleScene
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
	
	// タイトルの位置
	static constexpr POINT TITLE_POSITION{ (Screen::GAME_WIDTH - Tile::TILE_WIDTH * 11) / 2, Tile::TILE_HEIGHT * 2 };

	// GAMEの位置
	static constexpr POINT GAME_POSITION{ (Screen::GAME_WIDTH - Tile::TILE_WIDTH * 4) / 2, Tile::TILE_HEIGHT * 7 };

	// EDITの位置
	static constexpr POINT EDIT_POSITION{ (Screen::GAME_WIDTH - Tile::TILE_WIDTH * 4) / 2, Tile::TILE_HEIGHT * 10 };

	// 点滅の間隔
	static constexpr int BLINK_INTEVAL = 60;

// データメンバの宣言 -----------------------------------------------
private:

	// このシーンを含むゲームオブジェクトへのポインタ
	Game* m_pGame;

	// タイトルの文字列
	String m_titleString;

	// Gameの文字列
	String m_gameString;

	// Editの文字列
	String m_editString;

	// モード
	Mode m_mode;

	// 点滅制御
	Blink m_blink;

// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	TitleScene(Game* pGame);

	// デストラクタ
	~TitleScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(int keyCondition, int keyTrigger);

	// 描画処理
	void Render(int ghTileset);

	// 終了処理
	void Finalize();

};
