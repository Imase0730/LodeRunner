//--------------------------------------------------------------------------------------
// File: Guard.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Level.h"
#include "Game/Tile.h"

class GamePlayScene;

// ガードクラス
class Guard
{
public:

	// 移動方向
	enum class MoveDirection
	{
		None,
		UP,
		Down,
		Left,
		Right,
	};

	// 向き
	enum class Direction
	{
		Right,	// 右
		Left,	// 左
	};

	// アニメーションインデックス
	enum class GuardAnimationState
	{
		AnimNone,						// 表示しない
		Run01_L, Run02_L, Run03_L,		// 走り（左向き）
		Rope01_L, Rope02_L, Rope03_L,	// ロープで移動（左向き）
		Fall_L,							// 落下中（左向き）

		Run01_R, Run02_R, Run03_R,		// 走り（右向き）
		Rope01_R, Rope02_R, Rope03_R,	// ロープで移動（右向き）
		Fall_R,							// 落下中（右向き）

		Climb01, Climb02,				// ハシゴを登って
		Egg01, Egg02,					// 復活中
	};

	// クラス定数の宣言 -------------------------------------------------
private:

	// ガードの絵の位置
	static constexpr POINT GUARD_SPRITES[] =
	{
		// 表示しない
		{ 0, 4 },

		// 走り（左向き）
		{ 0, 7 },
		{ 1, 7 },
		{ 2, 7 },

		// ロープで移動（左向き）
		{ 0, 8 },
		{ 1, 8 },
		{ 2, 8 },

		// 落下中（左向き）
		{ 8, 7 },

		// 走り（右向き）
		{ 3, 7 },
		{ 4, 7 },
		{ 5, 7 },

		// ロープで移動（右向き）
		{ 3, 8 },
		{ 4, 8 },
		{ 5, 8 },

		// 落下中（右向き）
		{ 9, 7 },

		// ハシゴを上っている
		{ 6, 7 },
		{ 7, 7 },

		// 復活中
		{ 8, 10 },
		{ 9, 10 },
	};

	// 穴に落ちた時に揺れるアニメーションのスタートフレーム
	static constexpr int HOLE_ANIMATION_START_FRAME = 11;

	// 穴に落ちた時に横に揺れるアニメーションのテーブルサイズ
	static constexpr int HOLE_ANIMATION_TABLE_SIZE = 5;

	// 穴に落ちた時に横に揺れるアニメーションテーブル（Xの揺れ幅）
	static constexpr int HOLE_ANIMATION_TABLE[HOLE_ANIMATION_TABLE_SIZE] = { 2, 1, 2, 3, 2 };

	// 復活するまでの時間
	static constexpr int RESURRECTION_TIME = 0x26;

	// 復活アニメーション用
	static constexpr int RESURRECTION_ANIME_TIME_NONE = 20;
	static constexpr int RESURRECTION_ANIME_TIME_EGG01 = 19;
	static constexpr int RESURRECTION_ANIME_TIME_EGG02 = 10;

	// データメンバの宣言 -----------------------------------------------
private:

	// ゲームプレイシーンへのポインタ
	GamePlayScene* m_pGamePlayScene;

	// ステージへのポインタ
	Level* m_pLevel;

	// アクティブならtrueを設定
	bool m_isActive;

	// 生存フラグ
	bool m_isAlive;

	// 表示フラグ
	bool m_isDisplay;

	// タイル上の位置
	POINT m_tilePosition;

	// タイル内の調整用の位置
	POINT m_adjustPosition;

	// 顔の向き
	Direction m_faceDirection;

	// アニメーションステート
	GuardAnimationState m_guardAnimationState;

	// 行動タイマー（＋：穴に落ちているタイマー　０：通常　ー：金塊の保持しているタイマー）
	int m_actionStateTimer;

	// 復活タイマー
	int m_resurrectionTimer;

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Guard(GamePlayScene* pScene, Level* pLevel);

	// デストラクタ
	~Guard();

	// 初期化関数
	void Initialize(POINT tilePosition, POINT ajustPosition);

	// 更新処理
	void Update();

	// 描画関数
	void Render(int ghTileset);

	// アクティブを設定する関数
	void SetActive(bool active) { m_isActive = active; }

	// アクティブか調べる関数
	bool IsActive() const { return m_isActive; }

	// 生きているか調べる関数
	bool IsAlive() const { return m_isAlive; }

	// タイル上の位置を設定する関数
	void SetTilePosition(int colmun, int row)
	{
		m_tilePosition.x = colmun;
		m_tilePosition.y = row;
		SetAdjustPosition(Tile::TILE_CENTER_X, Tile::TILE_CENTER_Y);
	}

	// タイル上の位置を取得する関数
	POINT GetTilePosition() const { return m_tilePosition; }

	// タイル内の位置を設定する関数
	void SetAdjustPosition(int x, int y)
	{
		m_adjustPosition.x = x;
		m_adjustPosition.y = y;
	}

	// タイル内の位置を取得する関数
	POINT GetAdjustPosition() const { return m_adjustPosition; }

	// 行動タイマーを設定する関数
	void SetActionStateTimer(int goldTimer) { m_actionStateTimer = goldTimer; }

	// 行動タイマーを取得する関数
	int GetActionStateTimer() const { return m_actionStateTimer; }

	// 復活タイマーの設定関数
	void SetResurrectionTimer(int timer) { m_resurrectionTimer = timer; }

	// 復活タイマーの取得関数
	int GetResurrectionTimer() const { return m_resurrectionTimer; }

	// 指定位置から復活する関数
	void Resurrection(int colmun, int row);

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

	// 左に移動可能か調べる関数
	bool IsMovableLeft();

	// 右に移動可能か調べる関数
	bool IsMovableRight();

	// 落下中
	void Falling();

	// 移動方向を決める関数
	MoveDirection DecideMoveDirection();

	// 上に移動
	void MoveUp();

	// 下に移動
	void MoveDown();

	// 左に移動
	void MoveLeft();

	// 右に移動
	void MoveRight();

	// アニメーションステートの設定
	void SetGuardAnimationState(GuardAnimationState start, GuardAnimationState end);

	// 金塊が拾えるか調べる関数
	void CheckGoldPickedUp();

	// 指定した行まで移動可能か調べる関数
	bool IsMovableColumn(int column);
	
	// 左右どこまで移動できるか取得する関数
	void GetLeftRightLimits(int* colmun, Direction direction, int limit);

	// 下方向に移動な最適な行を見つける関数
	int FindCandidateRowBelow(int colmun, int row);

	// 上方向に移動な最適な行を見つける関数
	int FindCandidateRowAbove(int colmun, int row);

	// 左右に移動できそうか調べる関数
	bool CheckLeftRightMove(int colmun, int row, Direction direction);

	// プレイヤーとの疑似距離を求める関数
	int GetPsuedoDistance(int row);

	// 上下へ移動するか調べる関数
	Guard::MoveDirection SelectMoveUpAndDown(int* bestGuardDistance);

	// 左へ移動するか調べる関数
	Guard::MoveDirection SelectMoveLeftAndRight(int* bestGuardDistance, int leftColmunLimit, Direction direction);

	// 下に移動可能なタイルか調べる関数
	bool IsMovableDown(int colmun, int row);

	// 上に移動可能なタイルか調べる関数
	bool IsMovableUp(int colmun, int row);

	// 移動可能なタイルか調べる関数（落下）
	bool IsMovableTileFall(Tile::Type page1, Tile::Type page2);

	//ステートを更新して金塊を落とす処理
	void UpdateGoldDropTimer();

	// 移動可能なタイルか調べる関数（上左右）
	static bool IsMovableTileULR(Tile::Type tile);

	// 移動可能なタイルか調べる関数（下）
	static bool IsMovableTileD(Tile::Type tile);
	
	// 穴に落ちた時の処理
	bool HandleFallingInHole();

	// 落下処理
	bool HandlFalling();

	// 移動処理
	void HandleMovement();

	// 穴の中か調べる関数
	bool IsInHole() const;
};
