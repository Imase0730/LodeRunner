//--------------------------------------------------------------------------------------
// File: Player.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Stage.h"

// プレイヤークラス
class Player
{
public:

	// 向き
	enum class FaceDirection
	{
		Right,	// 右
		Left,	// 左
	};

	// アニメーションインデックス
	enum class AnimationState
	{
		Run01_L, Run02_L, Run03_L,		// 走り（左向き）
		Rope01_L, Rope02_L, Rope03_L,	// ロープで移動（左向き）
		Dig_L,							// 穴掘り（左向き）
		Fall_L,							// 落下中（左向き）

		Run01_R, Run02_R, Run03_R,		// 走り（右向き）
		Rope01_R, Rope02_R, Rope03_R,	// ロープで移動（右向き）
		Dig_R,							// 穴掘り（右向き）
		Fall_R,							// 落下中（右向き）

		Climb01, Climb02,				// ハシゴを登って
	};

	// 掘っている向き
	enum class DigDirection
	{
		NotDigging,	// 掘っていない
		Right,		// 右
		Left,		// 左
	};

	// クラス定数の宣言 -------------------------------------------------
private:

	// 掘っている最中のプレイヤー横のレンガの破片の位置（左向き）
	static constexpr POINT DIG_DEBRIS_LEFT_SPRITES[] =
	{
		{ 1, 9 }, { 1, 9 },
		{ 2, 9 }, { 2, 9 },
		{ 3, 9 }, { 3, 9 },
		{ 4, 9 }, { 4, 9 },
		{ -1, -1 }, { -1, -1 },
		{ -1, -1 }, { -1, -1 },
	};

	// 掘っている最中のプレイヤー横のレンガの破片の位置（右向き）
	static constexpr POINT DIG_DEBRIS_RIGHT_SPRITES[] =
	{
		{ 6, 9 }, { 6, 9 },
		{ 7, 9 }, { 7, 9 },
		{ 8, 9 }, { 8, 9 },
		{ 9, 9 }, { 9, 9 },
		{ -1, -1 }, { -1, -1 },
		{ -1, -1 }, { -1, -1 },
	};

	// 掘っている最中のプレイヤー斜め下のレンガの位置
	static constexpr POINT DIG_BRICK_SPRITES[] =
	{
		{ 0, 10 }, { 0, 10 },
		{ 1, 10 }, { 1, 10 },
		{ 2, 10 }, { 2, 10 },
		{ 3, 10 }, { 3, 10 },
		{ 4, 10 }, { 4, 10 },
		{ 5, 10 }, { 5, 10 },
	};

	// プレイヤーの位置
	static constexpr POINT PLAYER_SPRITES[] =
	{
		// 走り（左向き）
		{ 0, 5 },
		{ 1, 5 },
		{ 2, 5 },

		// ロープで移動（左向き）
		{ 0, 6 },
		{ 1, 6 },
		{ 2, 6 },

		// 穴掘り（左向き）
		{ 0, 9 },

		// 落下中（左向き）
		{ 8, 5 },

		// 走り（右向き）
		{ 3, 5 },
		{ 4, 5 },
		{ 5, 5 },

		// ロープで移動（右向き）
		{ 3, 6 },
		{ 4, 6 },
		{ 5, 6 },

		// 穴掘り（右向き）
		{ 5, 9 },

		// 落下中（右向き）
		{ 9, 5 },

		// ハシゴを上っている
		{ 6, 5 },
		{ 7, 5 },
	};

	// データメンバの宣言 -----------------------------------------------
private:

	// アクティブならtrueを設定
	bool m_isActive;

	// タイル上の位置
	POINT m_tilePosition;

	// タイル内の調整用の位置
	POINT m_ajustPosition;

	// 顔の向き
	FaceDirection m_faceDirection;

	// アニメーションステート
	AnimationState m_animationState;

	// 掘っている方向
	DigDirection m_digDirection;

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

	// 初期化関数
	void Initialize(POINT tilePosition, POINT ajustPosition);

	// 更新処理
	void Update(int keyCondition, int keyTrigger, Stage* pStage);

	// 描画関数
	void Render(int ghTileset) const;

	// アクティブを設定する関数
	void SetActive(bool active) { m_isActive = active; }

	// アクティブか調べる関数
	bool IsActive() const { return m_isActive; }

private:

	// 位置を列に調整する関数
	void AjustCloumn();

	// 位置を行に調整する関数
	void AjustRow();

	// 移動可能か調べる関数
	bool IsMovable(Stage* pStage) const;

	// 上に移動可能か調べる関数
	bool IsMovableUp(Stage* pStage) const;

	// 下に移動可能か調べる関数
	bool IsMovableDown(Stage* pStage) const;

	// 左に移動可能か調べる関数
	bool IsMovableLeft(Stage* pStage) const;

	// 右に移動可能か調べる関数
	bool IsMovableRight(Stage* pStage) const;

	// アニメーションステートの設定
	void SetAnimationState(AnimationState start, AnimationState end);

};
