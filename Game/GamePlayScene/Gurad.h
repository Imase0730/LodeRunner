//--------------------------------------------------------------------------------------
// File: Gurad.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Stage.h"

class GamePlayScene;
class Stage;

// ガードクラス
class Gurad
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
	enum class GuradAnimationState
	{
		Run01_L, Run02_L, Run03_L,		// 走り（左向き）
		Rope01_L, Rope02_L, Rope03_L,	// ロープで移動（左向き）
		Fall_L,							// 落下中（左向き）

		Run01_R, Run02_R, Run03_R,		// 走り（右向き）
		Rope01_R, Rope02_R, Rope03_R,	// ロープで移動（右向き）
		Fall_R,							// 落下中（右向き）

		Climb01, Climb02,				// ハシゴを登って
	};

	// クラス定数の宣言 -------------------------------------------------
private:

	// ガードの絵の位置
	static constexpr POINT GUARD_SPRITES[] =
	{
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
	};

	// データメンバの宣言 -----------------------------------------------
private:

	// ゲームプレイシーンへのポインタ
	GamePlayScene* m_pGamePlayScene;

	// ステージへのポインタ
	Stage* m_pStage;

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
	GuradAnimationState m_guardAnimationState;

	// 金塊保持タイマー
	int m_goldTimer;

	// 復活タイマー
	int m_resurrectionTimer;

	// 移動方向
	MoveDirection m_moveDirection;

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Gurad(GamePlayScene* pScene, Stage* pStage);

	// デストラクタ
	~Gurad();

	// 初期化関数
	void Initialize(POINT tilePosition, POINT ajustPosition);

	// 更新処理
	void Update();

	// 描画関数
	void Render(int ghTileset) const;

	// アクティブを設定する関数
	void SetActive(bool active) { m_isActive = active; }

	// アクティブか調べる関数
	bool IsActive() const { return m_isActive; }

	// 生存しているか調べる関数
	bool IsAlive() const { return m_isAlive; }

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
	bool IsFalling() const;

	// 上に移動可能か調べる関数
	bool IsMovableUp() const;

	// 下に移動可能か調べる関数
	bool IsMovableDown() const;

	// 左に移動可能か調べる関数
	bool IsMovableLeft() const;

	// 右に移動可能か調べる関数
	bool IsMovableRight() const;

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

	// ガードアニメーションステートの設定
	void SetGuradAnimationState(GuradAnimationState start, GuradAnimationState end);

	// 金塊が拾えるか調べる関数
	void CheckGoldPickedUp();

	// 指定した行まで移動可能か調べる関数
	bool IsMovableColumn(int column) const;
	
	// 左右どこまで移動できるか取得する関数
	void GetLeftRightLimits(int* colmun, Direction direction, int limit);

	// 下方向に移動な最適な行を見つける関数
	int FindCandidateRowBelow(int colmun, int row);

	// 上方向に移動な最適な行を見つける関数
	int FindCandidateRowAbove(int colmun, int row);

	// 左右に移動できそうか調べる関数
	bool CheckLeftRightMove(int colmun, int row, Direction direction);

	// プレイヤーとの疑似距離を求める関数
	int GetPseudoDistance(int colmun, int row);

	// 上下へ移動するか調べる関数
	void SelectMoveUpAndDown(int* bestGuradDistance);

	// 左へ移動するか調べる関数
	void SelectMoveLeftAndRight(int* bestGuradDistance, int leftColmunLimit, Direction direction);

	// 下に移動可能なタイルか調べる関数
	bool IsMovableDown(int colmun, int row);

	// 上に移動可能なタイルか調べる関数
	bool IsMovableUp(int colmun, int row);

};
