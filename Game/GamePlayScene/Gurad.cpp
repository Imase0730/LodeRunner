//--------------------------------------------------------------------------------------
// File: Gurad.cpp
//
//--------------------------------------------------------------------------------------
#include "Gurad.h"
#include "GamePlayScene.h"
#include "Game/Tile.h"

// コンストラクタ
Gurad::Gurad(GamePlayScene* pScene, Stage* pStage)
	: m_pGamePlayScene{ pScene }
	, m_pStage{ pStage }
	, m_isActive{ false }
	, m_isAlive{ false }
	, m_isDisplay{ false }
	, m_tilePosition{ 0, 0 }
	, m_adjustPosition{ 0, 0 }
	, m_faceDirection{ FaceDirection::Left }
	, m_guardAnimationState{ GuradAnimationState::Run01_L }
	, m_goldTimer{ 0 }
	, m_resurrectionTimer{ 0 }
{
}

// デストラクタ
Gurad::~Gurad()
{
}

// 初期化処理
void Gurad::Initialize(POINT tilePosition, POINT ajustPosition)
{
	// アクティブにする
	m_isActive = true;

	// 生存フラグ初期化
	m_isAlive = true;

	// 表示(ON)
	m_isDisplay = true;

	// 位置を初期化する
	m_tilePosition = tilePosition;
	m_adjustPosition = ajustPosition;

	// 各変数の初期化
	m_faceDirection = FaceDirection::Left;
	m_guardAnimationState = GuradAnimationState::Run01_L;
}

// 更新処理
void Gurad::Update()
{

	// 上下方向は はしごやロープがあれば上下移動可能
	// 左右は 障害物がなければ近づくように移動
	// ガードがとプレイヤーが同じ行にいる
	// OK
	// ハシゴ
	// ロープ
	// MAX_ROW(行）

	// プレイヤーと同じ行にいる場合
	// 左右へ移動　ハシゴやロープは移動可なので無視
	// 一番下の行でなく、一つ下が空白または罠があるなら移動しない
	// 条件を満たせば移動する



		// 左に移動
		MoveLeft();
		return;





	//// 今いる場所がレンガなら死亡
	//if (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Blick)
	//{
	//	// 死亡
	//	m_isAlive = false;
	//	// 表示(OFF)
	//	m_isDisplay = false;
	//	return;
	//}

	//// 落下中か
	//if (IsFalling())
	//{
	//	// 落下中
	//	Falling();
	//	return;
	//}

	//// 上キーが押された
	//if (keyCondition & PAD_INPUT_UP)
	//{
	//	// 上に移動
	//	MoveUp();
	//	return;
	//}

	//// 下キーが押された
	//if (keyCondition & PAD_INPUT_DOWN)
	//{
	//	// 下に移動
	//	MoveDown();
	//	return;
	//}

	//// 左キーが押された
	//if (keyCondition & PAD_INPUT_LEFT)
	//{
	//	// 左に移動
	//	MoveLeft();
	//	return;
	//}

	//// 右キーが押された
	//if (keyCondition & PAD_INPUT_RIGHT)
	//{
	//	// 右に移動
	//	MoveRight();
	//	return;
	//}
}

// 描画処理
void Gurad::Render(int ghTileset) const
{
	// 表示(OFF)
	if (!m_isDisplay) return;

	// ガードの描画
	POINT pos = GUARD_SPRITES[static_cast<int>(m_guardAnimationState)];
	DrawRectGraph( m_tilePosition.x * Tile::TILE_PIXEL_WIDTH + (m_adjustPosition.x - Tile::TILE_CENTER_X) * 2
				 , m_tilePosition.y * Tile::TILE_PIXEL_HEIGHT + (m_adjustPosition.y - Tile::TILE_CENTER_Y) * 2
				 , Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				 , Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
}

// 位置を列に調整する関数
void Gurad::AjustCloumn()
{
	// 調整する必要はなし
	if (m_adjustPosition.x == Tile::TILE_CENTER_X) return;

	// 左右にずれている場合は調整する
	if (m_adjustPosition.x < Tile::TILE_CENTER_X)
	{
		m_adjustPosition.x++;
	}
	else
	{
		m_adjustPosition.x--;
	}
	// 金塊が拾えるかチェック
	CheckGoldPickedUp();
}

// 位置を行に調整する関数
void Gurad::AjustRow()
{
	// 調整する必要はなし
	if (m_adjustPosition.y == Tile::TILE_CENTER_Y) return;

	// 上下にずれている場合は調整する
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y)
	{
		m_adjustPosition.y++;
	}
	else
	{
		m_adjustPosition.y--;
	}
	// 金塊が拾えるかチェック
	CheckGoldPickedUp();
}

// 落下しているかチェックする関数
bool Gurad::IsFalling() const
{
	// ハシゴなら移動可能
	if (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Ladder) return false;

	// ロープにつかんでいれば移動可能
	if ( (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Rope)
	  && (m_adjustPosition.y == Tile::TILE_CENTER_Y)
	   )
	{
		return false;
	}

	// 空中に浮いている
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y) return true;

	// 一番下なら移動可能
	if (m_tilePosition.y == Stage::STAGE_HEIGHT - 1) return false;

	// 下の行が落下可能なら
	if (Tile::IsMovableTileFall(m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y + 1))) return true;

	return false;
}

// 上に移動可能か調べる関数
bool Gurad::IsMovableUp() const
{
	// ハシゴがある？
	if (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Ladder)
	{
		// ハシゴ上なのでハシゴの真ん中までは移動可能
		if (m_adjustPosition.y > Tile::TILE_CENTER_Y) return true;

		// 一番上なら移動不可
		if (m_tilePosition.y == 0) return false;

		// 上の行が移動可能なら
		if (Tile::IsMovableTileULR(m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y - 1))) return true;
	}
	else
	{
		if (m_adjustPosition.y > Tile::TILE_CENTER_Y) return true;
	}
	return false;
}

bool Gurad::IsMovableDown() const
{
	// 少し上にいるので下に移動可能
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y) return true;

	// 一番下なら移動不可
	if (m_tilePosition.y == Stage::STAGE_HEIGHT - 1) return false;

	// 下の行が移動可能なら
	if (Tile::IsMovableTileDown(m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y + 1))) return true;

	return false;
}

// 左に移動可能か調べる関数
bool Gurad::IsMovableLeft() const
{
	// まだ左方向に移動させる余地がある
	if (m_adjustPosition.x > Tile::TILE_CENTER_X) return true;

	// 左隅なので移動不可
	if (m_tilePosition.x == 0) return false;

	// 移動先タイルが移動可能か調べる
	return Tile::IsMovableTileULR(m_pStage->GetTileType(m_tilePosition.x - 1, m_tilePosition.y));
}

// 右に移動可能か調べる関数
bool Gurad::IsMovableRight() const
{
	// まだ右方向に移動させる余地がある
	if (m_adjustPosition.x < Tile::TILE_CENTER_X) return true;

	// 右隅なので移動不可
	if (m_tilePosition.x == Stage::STAGE_WIDTH - 1) return false;

	// 移動先タイルが移動可能か調べる
	return Tile::IsMovableTileULR(m_pStage->GetTileType(m_tilePosition.x + 1, m_tilePosition.y));
}

// 落下中
void Gurad::Falling()
{
	// 列に合わせるようにX座標を調整する
	AjustCloumn();

	// 落下
	m_adjustPosition.y++;
	if (m_adjustPosition.y > 4)
	{
		m_adjustPosition.y = 0;
		m_tilePosition.y++;
	}

	// 金塊が拾えるかチェック
	CheckGoldPickedUp();

	// 落下アニメーション
	if (m_faceDirection == FaceDirection::Left)
	{
		m_guardAnimationState = GuradAnimationState::Fall_L;
	}
	else
	{
		m_guardAnimationState = GuradAnimationState::Fall_R;
	}
}

// 上に移動
void Gurad::MoveUp()
{
	// 上に移動可能か？
	if (IsMovableUp())
	{
		// 列に合わせるようにX座標を調整する
		AjustCloumn();

		// 上に移動する
		m_adjustPosition.y--;
		if (m_adjustPosition.y < 0)
		{
			m_adjustPosition.y = 4;
			m_tilePosition.y--;
		}

		// 金塊が拾えるかチェック
		CheckGoldPickedUp();

		// ハシゴを登るアニメーション
		SetGuradAnimationState(GuradAnimationState::Climb01, GuradAnimationState::Climb02);
	}
}

// 下に移動
void Gurad::MoveDown()
{
	// 下に移動可能か？
	if (IsMovableDown())
	{
		// 列に合わせるようにX座標を調整する
		AjustCloumn();

		// 下に移動する
		m_adjustPosition.y++;
		if (m_adjustPosition.y > 4)
		{
			m_adjustPosition.y = 0;
			m_tilePosition.y++;
		}

		// 金塊が拾えるかチェック
		CheckGoldPickedUp();

		// ハシゴを登るアニメーション
		SetGuradAnimationState(GuradAnimationState::Climb01, GuradAnimationState::Climb02);
	}
}

// 左に移動
void Gurad::MoveLeft()
{
	// 左に移動可能か？
	if (IsMovableLeft())
	{
		// 左向きに
		m_faceDirection = FaceDirection::Left;

		// 行に合わせるようにY座標を調整する
		AjustRow();

		// 左に移動する
		m_adjustPosition.x--;
		if (m_adjustPosition.x < 0)
		{
			m_adjustPosition.x = 4;
			m_tilePosition.x--;
		}

		// 金塊が拾えるかチェック
		CheckGoldPickedUp();

		// ロープの場合
		if (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Rope)
		{
			// ロープで移動するアニメーション
			SetGuradAnimationState(GuradAnimationState::Rope01_L, GuradAnimationState::Rope03_L);
		}
		else
		{
			// 走るアニメーション
			SetGuradAnimationState(GuradAnimationState::Run01_L, GuradAnimationState::Run03_L);
		}
	}
}

// 右に移動
void Gurad::MoveRight()
{
	// 右に移動可能か？
	if (IsMovableRight())
	{
		// 右向きに
		m_faceDirection = FaceDirection::Right;

		// 行に合わせるようにY座標を調整する
		AjustRow();

		// 右移動する
		m_adjustPosition.x++;
		if (m_adjustPosition.x > 4)
		{
			m_adjustPosition.x = 0;
			m_tilePosition.x++;
		}

		// 金塊が拾えるかチェック
		CheckGoldPickedUp();

		// ロープの場合
		if (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Rope)
		{
			// ロープで移動するアニメーション
			SetGuradAnimationState(GuradAnimationState::Rope01_R, GuradAnimationState::Rope03_R);
		}
		else
		{
			// 走るアニメーション
			SetGuradAnimationState(GuradAnimationState::Run01_R, GuradAnimationState::Run03_R);
		}
	}
}

// ガードアニメーションステートの設定
void Gurad::SetGuradAnimationState(GuradAnimationState beginAnimState, GuradAnimationState endAnimState)
{
	int begin = static_cast<int>(beginAnimState);
	int end = static_cast<int>(endAnimState);
	int animState = static_cast<int>(m_guardAnimationState);

	// 次のアニメーションへ最後まで終わったら始めに戻る
	animState++;

	// 別のアニメーションへ移行の場合は始めから
	if (animState < begin)
	{
		animState = begin;
	}
	else if (animState > end)
	{
		animState = begin;
	}

	m_guardAnimationState = static_cast<GuradAnimationState>(animState);
}

// 金塊が拾えるか調べる関数
void Gurad::CheckGoldPickedUp()
{
	// タイルの中心でなければチェックしない
	if (m_adjustPosition.x != Tile::TILE_CENTER_X) return;
	if (m_adjustPosition.y != Tile::TILE_CENTER_Y) return;

	// 金塊があれば金塊を拾う
	if (m_pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Gold)
	{
		// 空白にする
		m_pStage->SetTileType(m_tilePosition.x, m_tilePosition.y, Tile::TileType::Empty);
	}
}

