//--------------------------------------------------------------------------------------
// File: Stage.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Tile.h"

class Stage
{
	// 列挙型の宣言 -------------------------------------------------
public:

	// モード
	enum class Mode
	{
		GamePlay,
		StageEdit,
	};

	// 構造体の宣言 -------------------------------------------------
public:


	// クラス定数の宣言 -------------------------------------------------
public:

	// ステージデータのパス
	static constexpr char STAGE_DATA_PATH[] = "Resources/StageData/";

	// ステージサイズ
	static constexpr int STAGE_WIDTH = 28;
	static constexpr int STAGE_HEIGHT = 16;

	// ステージサイズ
	static constexpr int STAGE_SCREEN_WIDTH = STAGE_WIDTH * Tile::TILE_WIDTH;
	static constexpr int STAGE_SCREEN_HEIGHT = STAGE_HEIGHT * Tile::TILE_HEIGHT;

	// データメンバの宣言 -----------------------------------------------
private:

	// マップデータ
	Tile m_tileMap[STAGE_HEIGHT][STAGE_WIDTH];

	// モード
	Mode m_mode;

	// 表示中のレベル
	int m_level;

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Stage(Mode mode);

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

	// 指定場所のタイルを取得する関数
	Tile::TileType GetTileType(int x, int y) { return m_tileMap[y][x].GetTileType(); }

	// 指定レベルをセーブする関数
	bool SaveLevel(int level);

	// 指定レベルをロードする関数
	bool LoadLevel(int level);

	// レベルを取得する関数
	int GetLevel() const { return m_level; }

};

