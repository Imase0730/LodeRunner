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
		None,
		Blick,			// レンガ
		Ladder,			// ハシゴ
		Bar,			// バー
		Block,			// 壊せないブロック
		Gold,			// 金塊
		HiddenLadder,	// 隠れハシゴ
		Trap,			// トラップ
		Enemy,			// 敵
		Player,			// プレイヤー

		TileTypeMax,
	};

	// 構造体の宣言 -------------------------------------------------
public:


	// クラス定数の宣言 -------------------------------------------------
public:

	// タイルのサイズ
	static constexpr int TILE_WIDTH = 10;
	static constexpr int TILE_HEIGHT = 11;

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

};

