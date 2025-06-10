//--------------------------------------------------------------------------------------
// File: IrisWipe.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Level.h"

// アイリスワイプクラス
class IrisWipe
{
	// 列挙型の宣言 -------------------------------------------------
public:

	// モード
	enum class Mode
	{
		None,
		Open,	// オープン
		Close,	// クローズ
	};

	// クラス定数の宣言 -------------------------------------------------
private:

	// ワイプの中心位置
	static constexpr int WIPE_CENTER_X = Level::LEVEL_SCREEN_WIDTH / 2;
	static constexpr int WIPE_CENTER_Y = Level::LEVEL_SCREEN_HEIGHT / 2;

	// ワイプのフレーム数
	static constexpr int WIPE_COUNTER = 170;

	// ワイプのピクセルの幅
	static constexpr int WIPE_PIXEL_WIDTH = 4;
	static constexpr int WIPE_PIXEL_HEIGHT = 1;

	// データメンバの宣言 -----------------------------------------------
private:

	// マスクデータ
	static unsigned char m_maskData[Level::LEVEL_SCREEN_HEIGHT][Level::LEVEL_SCREEN_WIDTH];

	// ワイプ用使マスク用中フラグ
	bool m_isUse;

	// ワイプ用マスク動作中フラグ
	bool m_isActive;

	// ワイプモード
	Mode m_mode;

	// ワイプのカウンター
	int m_counter;

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	IrisWipe();

	// デストラクタ
	~IrisWipe();

	// 初期化処理
	void Initialize(Mode mode);

	// 更新処理
	void Update();

	// 描画処理
	void Render() const;

	// 終了処理
	void Finalize();

	// ワイプを開始する関数
	void Start(Mode mode);

	// ワープのモードを取得する関数
	Mode GetMode() const { return m_mode; }

	// ワイプ動作中か調べる関数
	bool IsActive() const { return m_isActive; }

private:

	// カウンターの更新関数
	int UpdateCounter();

};
