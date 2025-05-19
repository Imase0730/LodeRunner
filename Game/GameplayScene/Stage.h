//--------------------------------------------------------------------------------------
// File: Stage.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Tile.h"

// クラスの前方宣言
class GameplayScene;

class Stage
{
	// 列挙型の宣言 -------------------------------------------------
public:


	// 構造体の宣言 -------------------------------------------------
public:


	// クラス定数の宣言 -------------------------------------------------
public:

	// ステージサイズ
	static constexpr int STAGE_WIDTH = 28;
	static constexpr int STAGE_HEIGHT = 16;

	// データメンバの宣言 -----------------------------------------------
private:

	// ゲームシーンへのポインタ
	GameplayScene* m_pGameplayScene;

	// グラフィックハンドル
	int m_ghGame_bottom;
	int m_ghEdit_bottom;

	// マップデータ
	Tile m_tileMap[STAGE_HEIGHT][STAGE_WIDTH];


	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Stage(GameplayScene* scene);

	// デストラクタ
	~Stage();

	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Render(int ghTileset) const;

	// 指定場所のタイルを設定する関数
	void SetTileType(int x, int y, Tile::TileType type) { m_tileMap[y][x].SetTileType(type); }
};

