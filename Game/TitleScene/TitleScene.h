//--------------------------------------------------------------------------------------
// File: TitleScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Game/StringRenderer.h"
#include "Game/Blink.h"
#include "Game/Level.h"
#include "Game/Tile.h"

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
		Game,	// ゲーム
		Edit,	// エディット
		Score,	// スコア
	};

// クラス定数の宣言 -------------------------------------------------
private:
	
	// タイトルの位置
	static constexpr POINT TITLE_POSITION{ (Screen::GAME_WIDTH - Tile::TILE_PIXEL_WIDTH * 11) / 2, Tile::TILE_PIXEL_HEIGHT * 2 };

	// GAMEの位置
	static constexpr POINT GAME_POSITION{ (Screen::GAME_WIDTH - Tile::TILE_PIXEL_WIDTH * 4) / 2, Tile::TILE_PIXEL_HEIGHT * 6 };

	// EDITの位置
	static constexpr POINT EDIT_POSITION{ (Screen::GAME_WIDTH - Tile::TILE_PIXEL_WIDTH * 4) / 2, Tile::TILE_PIXEL_HEIGHT * 9 };

	// SCOREの位置
	static constexpr POINT SCORE_POSITION{ (Screen::GAME_WIDTH - Tile::TILE_PIXEL_WIDTH * 5) / 2, Tile::TILE_PIXEL_HEIGHT * 12 };

	// 点滅の間隔
	static constexpr int BLINK_INTERVAL = 60;

// データメンバの宣言 -----------------------------------------------
private:

	// このシーンを含むゲームオブジェクトへのポインタ
	Game* m_pGame;

	// タイトルの文字列
	StringRenderer m_titleStringRenderer;

	// Gameの文字列
	StringRenderer m_gameStringRenderer;

	// Editの文字列
	StringRenderer m_editStringRenderer;

	// Scoreの文字列
	StringRenderer m_scoreStringRenderer;

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
