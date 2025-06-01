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

	// 構造体の宣言 -------------------------------------------------
public:


	// クラス定数の宣言 -------------------------------------------------
public:

	// タイルのサイズ
	static constexpr int TILE_WIDTH = 10;
	static constexpr int TILE_HEIGHT = 11;

	// タイルの中心までの距離
	static constexpr int CENTER_OFFSET_X = TILE_WIDTH / 2;
	static constexpr int CENTER_OFFSET_Y = TILE_HEIGHT / 2;

	// データメンバの宣言 -----------------------------------------------
private:

	// タイルタイプ
	TileType m_tileType;

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Tile();

	// デストラクタ
	~Tile();

	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Render(int x, int y, int ghTileset) const;

	// タイルタイプの設定関数
	void SetTileType(TileType type) { m_tileType = type; }

	// タイルタイプの取得関数
	TileType GetTileType() const { return m_tileType; }

	// タイルが移動可能か調べる関数
	static bool IsMovableTile(TileType tileType);

};

