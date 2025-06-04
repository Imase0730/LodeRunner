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
	static constexpr int STAGE_SCREEN_WIDTH = STAGE_WIDTH * Tile::TILE_PIXEL_WIDTH;
	static constexpr int STAGE_SCREEN_HEIGHT = STAGE_HEIGHT * Tile::TILE_PIXEL_HEIGHT;

	// ステージ上に置ける敵の最大数
	static constexpr int ENEMY_MAX = 5;

	// データメンバの宣言 -----------------------------------------------
private:

	// 読み込んだステージデータ
	Tile::TileType m_loadData[STAGE_HEIGHT][STAGE_WIDTH];

	// ステージデータ
	Tile m_stageData[STAGE_HEIGHT][STAGE_WIDTH];

	// モード
	Mode m_mode;

	// 表示中のレベル
	int m_level;

	// ステージ上の敵の数
	int m_enemyCount;

	// プレイヤーの初期位置
	POINT m_playerPosition;

	// 敵の初期位置
	POINT m_enemyPosition[ENEMY_MAX];

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Stage(Mode mode);

	// デストラクタ
	~Stage();

	// 初期化
	void Initialize(Mode mode);

	// 更新処理
	void Update();

	// 描画処理
	void Render(int ghTileset) const;

	// 指定場所のタイルを設定する関数
	void SetTileType(int x, int y, Tile::TileType type) { m_stageData[y][x].SetTileType(type); }

	// 指定場所のタイルの掘るアニメーションを設定する関数
	void SetTileDigAnimationState(int x, int y, Tile::DigAnimationState state) { m_stageData[y][x].SetDigAnimationState(state); }

	// 指定場所のタイルを取得する関数
	Tile::TileType GetTileType(int x, int y);

	// 指定レベルをセーブする関数
	bool SaveLevel(int level);

	// 指定レベルをロードする関数
	bool LoadLevel(int level, Mode mode);

	// レベルを取得する関数
	int GetLevel() const { return m_level; }

	// ハシゴを出現する関数
	void AppearLadder();

	// プレイヤーの位置を取得する関数
	POINT GetPlayerPosition() const { return m_playerPosition; }

};

