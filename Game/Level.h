//--------------------------------------------------------------------------------------
// File: Level.h
//
//--------------------------------------------------------------------------------------
#pragma once

class Level
{
	// 列挙型の宣言 -------------------------------------------------
public:

	// モード
	enum class Mode
	{
		GamePlay,
		LevelEdit,
	};

	// タイル
	enum class Tile
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

	// 掘るアニメーションステート
	enum class DigAnimationState
	{
		NotDigging = -1,
		Dig01, Dig02, Dig03, Dig04, Dig05, Dig06,
		Dig07, Dig08, Dig09, Dig10, Dig11, Dig12,
		Fill01, Fill02,
	};

	// 構造体の宣言 -------------------------------------------------
private:

	// 掘ったレンガの情報
	struct DigBrick
	{
		POINT position;	// 位置
		int timer;		// 復元タイマー
	};

	// クラス定数の宣言 -------------------------------------------------
public:

	// ステージデータのパス
	static constexpr char LEVEL_DATA_PATH[] = "Resources/LevelData/";

	// タイルのピクセルサイズ（表示用）
	static constexpr int TILE_PIXEL_WIDTH = 10;
	static constexpr int TILE_PIXEL_HEIGHT = 11;

	// タイルのサイズ（プログラム上での座標計算用）
	static constexpr int TILE_WIDTH = 5;
	static constexpr int TILE_HEIGHT = 5;

	// タイルの中心までの距離（プログラム上での座標計算用）
	static constexpr int TILE_CENTER_X = 2;
	static constexpr int TILE_CENTER_Y = 2;

	// レベルサイズ
	static constexpr int MAX_GAME_COLMUN = 28 - 1;
	static constexpr int MAX_GAME_ROW = 16 - 1;

	// ステージサイズ
	static constexpr int LEVEL_SCREEN_WIDTH = (MAX_GAME_COLMUN + 1) * TILE_PIXEL_WIDTH;
	static constexpr int LEVEL_SCREEN_HEIGHT = (MAX_GAME_ROW + 1) * TILE_PIXEL_HEIGHT;

	// ステージ上に置けるガードの最大数
	static constexpr int GUARD_MAX = 5;

	// 掘ったレンガ情報を記憶するワークの最大数
	static constexpr int DIG_BRICK_MAX = 30;

	// 掘ったレンガの復元するまでのフレーム数
	static constexpr int BRICK_FILL_FRAME = 180;

	// レンガの復元アニメーション用
	static constexpr int BRICK_ANIME_TIME_FILL01 = 20;
	static constexpr int BRICK_ANIME_TIME_FILL02 = 10;

	// タイルの絵の位置
	static constexpr POINT TILE_SPRITES[] =
	{
		{ 0, 4 },	// なし
		{ 1, 4 },	// レンガ
		{ 2, 4 },	// 石
		{ 3, 4 },	// ハシゴ
		{ 4, 4 },	// ロープ
		{ 5, 4 },	// トラップ
		{ 6, 4 },	// 隠れハシゴ
		{ 7, 4 },	// 金塊
		{ 8, 4 },	// ガード
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

	// 読み込んだレベルデータ
	Tile m_loadData[MAX_GAME_ROW + 1][MAX_GAME_COLMUN + 1];

	// PAGE1
	Tile m_page1[MAX_GAME_ROW + 1][MAX_GAME_COLMUN + 1];

	// PAGE2
	Tile m_page2[MAX_GAME_ROW + 1][MAX_GAME_COLMUN + 1];

	// モード
	Mode m_mode;

	// レベル番号
	int m_levelNo;

	// ステージ上のガードの数
	int m_guardCount;

	// ステージ上の金塊の数
	int m_goldCount;

	// プレイヤーの初期位置
	POINT m_playerPosition;

	// ガードの初期位置
	POINT m_guardPosition[GUARD_MAX];

	// 掘ったレンガの情報記録用
	DigBrick m_digBrick[DIG_BRICK_MAX];

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Level(Mode mode);

	// デストラクタ
	~Level();

	// 初期化
	void Initialize(int levelNo, Mode mode);

	// 更新処理
	void Update();

	// 描画処理
	void Render(int ghTileset) const;

	// レベル番号の取得
	int GetLevel() const { return m_levelNo; }

	// タイルを設定する関数（Page1）
	void SetTilePage1(int x, int y, Tile tile) { m_page1[y][x] = tile; }

	// タイルを設定する関数（Page2）
	void SetTilePage2(int x, int y, Tile tile) { m_page2[y][x] = tile; }

	// タイルを取得する関数（Page1）
	Tile GetTilePage1(int x, int y) const { return m_page1[y][x]; }

	// タイルを取得する関数（Page2）
	Tile GetTilePage2(int x, int y) const { return m_page2[y][x]; }

	//// 指定場所のタイルの掘るアニメーションを設定する関数
	//void SetTileDigAnimationState(int x, int y, Tile::DigAnimationState state) { m_stageData[y][x].SetDigAnimationState(state); }

	// 指定レベルをセーブする関数
	bool SaveLevel(int level);

	// 指定レベルをロードする関数
	bool LoadLevel(int level, Mode mode);

	// ハシゴを出現する関数
	void AppearLadder();

	// プレイヤーの位置を取得する関数
	POINT GetPlayerPosition() const { return m_playerPosition; }

	// 指定位置のレンガを復元する
	void SetFillBrick(int x, int y);

	// ステージ上の金塊の数を取得する関数
	int GetGoldCount() const { return m_goldCount; }

	// ステージ上の金塊の数を減らす関数
	void GetGold() { m_goldCount--; }

	// ステージ上のガードの数を取得する関数
	int GetGuardCount() const { return m_guardCount; }

	// ガードの位置を取得する関数
	POINT GetGuardPosition(int index) const { return m_guardPosition[index]; }

};

