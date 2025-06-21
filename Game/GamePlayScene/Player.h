//--------------------------------------------------------------------------------------
// File: Player.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Level.h"
#include "Game/Tile.h"

class GamePlayScene;

// プレイヤークラス
class Player
{
public:

	// 向き
	enum class Direction
	{
		Right,	// 右
		Left,	// 左
	};

	// アニメーションステート
	enum class PlayerAnimationState
	{
		AnimNone,						// 表示しない
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

	// 掘るアニメーションステート
	enum class DigAnimationState
	{
		NotDigging = -1,
		Dig01, Dig02, Dig03, Dig04, Dig05, Dig06,
		Dig07, Dig08, Dig09, Dig10, Dig11, Dig12,
	};

	// クラス定数の宣言 -------------------------------------------------
private:

	// プレイヤーの絵の位置
	static constexpr POINT PLAYER_SPRITES[] =
	{
		// 表示しない
		{ 0, 4 },

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

	// 掘られているレンガの絵の位置
	static constexpr POINT DIG_BRICK_SPRITES[] =
	{
		{ 0, 10 }, { 0, 10 },
		{ 1, 10 }, { 1, 10 },
		{ 2, 10 }, { 2, 10 },
		{ 3, 10 }, { 3, 10 },
		{ 4, 10 }, { 4, 10 },
		{ 5, 10 }, { 5, 10 },
	};

	// 掘っている最中のレンガの破片の絵の位置（左向き）
	static constexpr POINT DIG_DEBRIS_LEFT_SPRITES[] =
	{
		{ 1, 9 }, { 1, 9 },
		{ 2, 9 }, { 2, 9 },
		{ 3, 9 }, { 3, 9 },
		{ 4, 9 }, { 4, 9 },
		{ -1, -1 }, { -1, -1 },
		{ -1, -1 }, { -1, -1 },
	};

	// 掘っている最中のレンガの破片の絵の位置（右向き）
	static constexpr POINT DIG_DEBRIS_RIGHT_SPRITES[] =
	{
		{ 6, 9 }, { 6, 9 },
		{ 7, 9 }, { 7, 9 },
		{ 8, 9 }, { 8, 9 },
		{ 9, 9 }, { 9, 9 },
		{ -1, -1 }, { -1, -1 },
		{ -1, -1 }, { -1, -1 },
	};

	// データメンバの宣言 -----------------------------------------------
private:

	// ゲームプレイシーンへのポインタ
	GamePlayScene* m_pGamePlayScene;

	// レベルへのポインタ
	Level* m_pLevel;

	// アクティブならtrueを設定
	bool m_isActive;

	// 生存フラグ
	bool m_isAlive;

	// 表示フラグ
	bool m_isVisible;

	// タイル上の位置
	POINT m_tilePosition;

	// タイル内の調整用の位置
	POINT m_adjustPosition;

	// 顔の向き
	Direction m_faceDirection;

	// アニメーションステート
	PlayerAnimationState m_playerAnimationState;

	// 掘る方向
	DigDirection m_digDirection;

	// 掘るアニメーションステート
	DigAnimationState m_digAnimationState;

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Player(GamePlayScene* pScene, Level* pLevel);

	// デストラクタ
	~Player();

	// 初期化関数
	void Initialize(POINT tilePosition, POINT ajustPosition);

	// 更新処理
	void Update(int keyCondition, int keyTrigger);

	// 描画関数
	void Render(int ghTileset) const;

	// アクティブを設定する関数
	void SetActive(bool active) { m_isActive = active; }

	// アクティブか調べる関数
	bool IsActive() const { return m_isActive; }

	// 生きているか設定する関数
	void SetAlive(bool alive);

	// 生存しているか調べる関数
	bool IsAlive() const { return m_isAlive; }

	// 表示(ON/OFF)
	void SetVisible(bool isVisible) { m_isVisible = isVisible; }

	// タイル上の位置を取得する関数
	POINT GetTilePosition() const { return m_tilePosition; }

	// タイル内の位置を取得する関数
	POINT GetAdjustPosition() const { return m_adjustPosition; }

private:

	// 位置を列に調整する関数
	void AjustCloumn();

	// 位置を行に調整する関数
	void AjustRow();

	// 落下しているかチェックする関数
	bool IsFalling();

	// 上に移動可能か調べる関数
	bool IsMovableUp();

	// 下に移動可能か調べる関数
	bool IsMovableDown();

	// 左に掘れるか調べる関数
	bool IsDiggableLeft();

	// 右に掘れるか調べる関数
	bool IsDiggableRight();

	// 左に移動可能か調べる関数
	bool IsMovableLeft();

	// 右に移動可能か調べる関数
	bool IsMovableRight();

	// 左に掘る
	void DigLeft();

	// 右に掘る
	void DigRight();

	// 落下中
	void Falling();

	// 上に移動
	void MoveUp();

	// 下に移動
	void MoveDown();

	// 左に移動
	void MoveLeft();

	// 右に移動
	void MoveRight();

	// プレイヤーアニメーションステートの設定
	void SetPlayerAnimationState(PlayerAnimationState start, PlayerAnimationState end);

	// 掘るアニメーションの更新
	void UpdateDigAnimation();

	// 金塊が拾えるか調べる関数
	void CheckGoldPickedUp();

	// 移動可能なタイルか調べる関数（落下）
	bool IsMovableTileFall(Tile::Type page1, Tile::Type page2);

	// 移動可能なタイルか調べる関数（上左右）
	static bool IsMovableTileULR(Tile::Type tile);

	// 移動可能なタイルか調べる関数（下）
	static bool IsMovableTileDown(Tile::Type tile);

};
