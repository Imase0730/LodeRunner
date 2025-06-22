//--------------------------------------------------------------------------------------
// File: Tile.h
//
//--------------------------------------------------------------------------------------
#pragma once

// タイルクラス
class Tile
{
	// 列挙型の宣言 -------------------------------------------------
public:

	// タイルのタイプ
	enum class Type
	{
		Empty,				// なし
		Blick,				// レンガ
		Stone,				// 石
		Ladder,				// ハシゴ
		Rope,				// ロープ
		Trap,				// トラップ
		InvisibleLadder,	// 隠れハシゴ
		Gold,				// 金塊
		Guard,				// ガード
		Player,				// プレイヤー
	};

	// クラス定数の宣言 ---------------------------------------------
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

};
