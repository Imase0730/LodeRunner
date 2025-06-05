//--------------------------------------------------------------------------------------
// File: Tile.h
//
//--------------------------------------------------------------------------------------
#pragma once

class Tile
{
	// 列挙型の宣言 -------------------------------------------------
public:

	// タイルタイプ
	enum class TileType
	{
		Empty,				// なし
		Blick,				// レンガ
		Stone,				// 壊せないブロック
		Ladder,				// ハシゴ
		Rope,				// ロープ
		Trap,				// トラップ
		InvisibleLadder,	// 隠れハシゴ
		Gold,				// 金塊
		Enemy,				// 敵
		Player,				// プレイヤー

		TileTypeMax,
	};

	// 掘るアニメーションインデックス
	enum class DigAnimationState
	{
		NotDigging = -1,
		Dig01, Dig02, Dig03, Dig04, Dig05, Dig06,
		Dig07, Dig08, Dig09, Dig10, Dig11, Dig12,
		Fill01, Fill02,
	};

	// 構造体の宣言 -------------------------------------------------
public:


	// クラス定数の宣言 -------------------------------------------------
public:

	// タイルのピクセルサイズ（表示用）
	static constexpr int TILE_PIXEL_WIDTH = 10;
	static constexpr int TILE_PIXEL_HEIGHT = 11;

	// タイルのサイズ（プログラム上での座標計算用）
	static constexpr int TILE_WIDTH = 5;
	static constexpr int TILE_HEIGHT = 5;

	// タイルの中心までの距離（プログラム上での座標計算用）
	static constexpr int TILE_CENTER_X = 2;
	static constexpr int TILE_CENTER_Y = 2;

	// タイルの絵の位置
	static constexpr POINT TILE_SPRITES[] =
	{
		{ 0, 4 },	// なし
		{ 1, 4 },	// レンガ
		{ 2, 4 },	// 壊せないブロック
		{ 3, 4 },	// ハシゴ
		{ 4, 4 },	// ロープ
		{ 5, 4 },	// トラップ
		{ 6, 4 },	// 隠れハシゴ
		{ 7, 4 },	// 金塊
		{ 8, 4 },	// 敵
		{ 9, 4 },	// プレイヤー
	};

	// 掘られているレンガの絵の位置
	static constexpr POINT DIG_BRICK_SPRITES[] =
	{
		{ 0, 10 }, { 0, 10 },
		{ 1, 10 }, { 1, 10 },
		{ 2, 10 }, { 2, 10 },
		{ 3, 10 }, { 3, 10 },
		{ 4, 10 }, { 4, 10 },
		{ 5, 10 }, { 5, 10 },

		{ 6, 10 }, { 7, 10 },
	};

	// データメンバの宣言 -----------------------------------------------
private:

	// タイルタイプ
	TileType m_tileType;

	// 掘るアニメーションステート
	DigAnimationState m_digAnimationState;

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Tile();

	// 描画処理
	void Render(int x, int y, int ghTileset) const;

	// タイルタイプの設定関数
	void SetTileType(TileType type) { m_tileType = type; }

	// タイルタイプの取得関数
	TileType GetTileType() const { return m_tileType; }

	// 掘るアニメーションステートの取得関数
	DigAnimationState GetDigAnimationState() const { return m_digAnimationState; }

	// 掘るアニメーションステートの設定関数
	void SetDigAnimationState(DigAnimationState state) { m_digAnimationState = state; }

	// 移動可能なタイルか調べる関数（上左右）
	static bool IsMovableTileULR(TileType tileType);

	// 移動可能なタイルか調べる関数（下）
	static bool IsMovableTileDown(TileType tileType);

	// 移動可能なタイルか調べる関数（落下）
	static bool IsMovableTileFall(TileType tileType);

};

