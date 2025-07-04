//--------------------------------------------------------------------------------------
// File: Level.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Screen.h"
#include "Tile.h"

class Level
{
	// 列挙型の宣言 -----------------------------------------------------
public:

	// モード
	enum class Mode
	{
		GamePlay,
		LevelEdit,
	};

	// クラス定数の宣言 -------------------------------------------------
public:

	// ステージデータのパス
	static constexpr char LEVEL_DATA_PATH[] = "Resources/LevelData/";

	// レベルサイズ
	static constexpr int MAX_GAME_COLMUN = 28 - 1;
	static constexpr int MAX_GAME_ROW = 16 - 1;

	// ステージサイズ
	static constexpr int LEVEL_SCREEN_WIDTH = (MAX_GAME_COLMUN + 1) * Tile::TILE_PIXEL_WIDTH;
	static constexpr int LEVEL_SCREEN_HEIGHT = (MAX_GAME_ROW + 1) * Tile::TILE_PIXEL_HEIGHT;

	// ステージ上に置けるガードの最大数
	static constexpr int GUARD_MAX = 5;

	// ステージ上に置ける隠れハシゴの最大数
	static constexpr int INVISIBLE_LADDER_MAX = 45;

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

	// 画面下部の情報ウインドウの縦の表示位置
	static constexpr int INFOMATION_Y = Screen::GAME_HEIGHT - Tile::TILE_PIXEL_HEIGHT;

	// データメンバの宣言 -----------------------------------------------
private:

	// 読み込んだレベルデータ
	Tile::Type m_loadData[MAX_GAME_ROW + 1][MAX_GAME_COLMUN + 1];

	// PAGE1
	Tile::Type m_page1[MAX_GAME_ROW + 1][MAX_GAME_COLMUN + 1];

	// PAGE2
	Tile::Type m_page2[MAX_GAME_ROW + 1][MAX_GAME_COLMUN + 1];

	// モード
	Mode m_mode;

	// レベル番号
	int m_levelId;

	// ステージ上のガードの数
	int m_guardCount;

	// ステージ上の金塊の数
	int m_goldCount;

	// ステージ上の隠れハシゴの数
	int m_invisibleLadderCount;

	// プレイヤーの初期位置
	POINT m_playerPosition;

	// ガードの初期位置
	POINT m_guardPosition[GUARD_MAX];

	// 隠れハシゴの情報記録用
	POINT m_invisibleLadderPosition[INVISIBLE_LADDER_MAX];

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Level(Mode mode);

	// デストラクタ
	~Level();

	// 初期化
	bool Initialize(Mode mode);

	// 更新処理
	void Update();

	// 描画処理
	void Render(int ghTileset) const;

	// レベル番号の取得
	int GetLevel() const { return m_levelId; }

	// タイルを設定する関数（Page1）
	void SetTilePage1(int x, int y, Tile::Type tile) { m_page1[y][x] = tile; }

	// タイルを設定する関数（Page2）
	void SetTilePage2(int x, int y, Tile::Type tile) { m_page2[y][x] = tile; }

	// タイルを取得する関数（Page1）
	Tile::Type GetTilePage1(int x, int y) const { return m_page1[y][x]; }

	// タイルを取得する関数（Page2）
	Tile::Type GetTilePage2(int x, int y) const { return m_page2[y][x]; }

	// 指定レベルをセーブする関数
	bool SaveLevel(int level);

	// 指定レベルをロードする関数
	bool LoadLevel(int level);

	// 隠しハシゴを出現する関数
	void AppearLadder();

	// プレイヤーの位置を取得する関数
	POINT GetPlayerPosition() const { return m_playerPosition; }

	// ステージ上の金塊の数を取得する関数
	int GetGoldCount() const { return m_goldCount; }

	// ステージ上の金塊の数を減らす関数
	void LostGold() { m_goldCount--; }

	// ステージ上のガードの数を取得する関数
	int GetGuardCount() const { return m_guardCount; }

	// ガードの位置を取得する関数
	POINT GetGuardPosition(int index) const { return m_guardPosition[index]; }

	// Page2の内容をPage1に指定位置のタイルをコピーする関数
	void CopyPage2toPage1(int x, int y);

	// レベルデータが適正か調べる関数
	bool CheckLevelData() const;

	// Page2を取得する関数（Page2）
	Tile::Type (*GetPage2())[Level::MAX_GAME_ROW + 1][Level::MAX_GAME_COLMUN + 1];

	// ロードデータの設定する関数
	void SetLoadData(Tile::Type data[][Level::MAX_GAME_COLMUN + 1]);

};

