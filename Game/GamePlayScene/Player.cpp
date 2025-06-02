//--------------------------------------------------------------------------------------
// File: Player.cpp
//
//--------------------------------------------------------------------------------------
#include "Player.h"
#include "Game/Tile.h"

// コンストラクタ
Player::Player()
	: m_isActive{ false }
	, m_tilePosition{ 0, 0 }
	, m_ajustPosition{ 0, 0 }
	, m_faceDirection{ FaceDirection::Right }
	, m_animationState{ AnimationState::Run01_R }
{
}

// デストラクタ
Player::~Player()
{
}

// 初期化処理
void Player::Initialize(POINT tilePosition, POINT ajustPosition)
{
	// アクティブにする
	m_isActive = true;

	// 位置を初期化する
	m_tilePosition = tilePosition;
	m_ajustPosition = ajustPosition;

	// プレイヤーの向き
	m_faceDirection = FaceDirection::Right;
}

// 更新処理
void Player::Update(int keyCondition, int keyTrigger, Stage* pStage)
{
	// 移動可能じゃない
	if (!IsMovable(pStage))
	{
		// 列に合わせるようにX座標を調整する
		AjustCloumn();

		// 落下
		m_ajustPosition.y++;
		if (m_ajustPosition.y > 4)
		{
			m_ajustPosition.y = 0;
			m_tilePosition.y++;
		}

		// 落下アニメーション
		if (m_faceDirection == FaceDirection::Left)
		{
			m_animationState = AnimationState::Fall_L;
		}
		else
		{
			m_animationState = AnimationState::Fall_R;
		}
		return;
	}

	// 上キーが押された
	if (keyCondition & PAD_INPUT_UP)
	{
		// 上に移動可能か？
		if (IsMovableUp(pStage))
		{
			// 列に合わせるようにX座標を調整する
			AjustCloumn();

			// 上に移動する
			m_ajustPosition.y--;
			if (m_ajustPosition.y < 0)
			{
				m_ajustPosition.y = 4;
				m_tilePosition.y--;
			}

			// ハシゴを登るアニメーション
			SetAnimationState(AnimationState::Climb01, AnimationState::Climb02);
		}
	}

	// 下キーが押された
	if (keyCondition & PAD_INPUT_DOWN)
	{
		// 下に移動可能か？
		if (IsMovableDown(pStage))
		{
			// 列に合わせるようにX座標を調整する
			AjustCloumn();

			// 下に移動する
			m_ajustPosition.y++;
			if (m_ajustPosition.y > 4)
			{
				m_ajustPosition.y = 0;
				m_tilePosition.y++;
			}

			// ハシゴを登るアニメーション
			SetAnimationState(AnimationState::Climb01, AnimationState::Climb02);
		}
	}

	// 左キーが押された
	if (keyCondition & PAD_INPUT_LEFT)
	{
		// 左に移動可能か？
		if (IsMovableLeft(pStage))
		{
			// 左向きに
			m_faceDirection = FaceDirection::Left;
			
			// 行に合わせるようにY座標を調整する
			AjustRow();

			// 左に移動する
			m_ajustPosition.x--;
			if (m_ajustPosition.x < 0)
			{
				m_ajustPosition.x = 4;
				m_tilePosition.x--;
			}

			// ロープの場合
			if (pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Rope)
			{
				// ロープで移動するアニメーション
				SetAnimationState(AnimationState::Rope01_L, AnimationState::Rope03_L);
			}
			else
			{
				// 走るアニメーション
				SetAnimationState(AnimationState::Run01_L, AnimationState::Run03_L);
			}
		}
	}

	// 右キーが押された
	if (keyCondition & PAD_INPUT_RIGHT)
	{
		// 右に移動可能か？
		if (IsMovableRight(pStage))
		{
			// 右向きに
			m_faceDirection = FaceDirection::Right;

			// 行に合わせるようにY座標を調整する
			AjustRow();

			// 右移動する
			m_ajustPosition.x++;
			if (m_ajustPosition.x > 4)
			{
				m_ajustPosition.x = 0;
				m_tilePosition.x++;
			}

			// ロープの場合
			if (pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Rope)
			{
				// ロープで移動するアニメーション
				SetAnimationState(AnimationState::Rope01_R, AnimationState::Rope03_R);
			}
			else
			{
				// 走るアニメーション
				SetAnimationState(AnimationState::Run01_R, AnimationState::Run03_R);
			}
		}
	}
}

// 描画処理
void Player::Render(int ghTileset) const
{
	POINT pos = SPRITE_POSITION[static_cast<int>(m_animationState)];
	DrawRectGraph( m_tilePosition.x * Tile::TILE_PIXEL_WIDTH + (m_ajustPosition.x - Tile::TILE_CENTER_X) * 2
				 , m_tilePosition.y * Tile::TILE_PIXEL_HEIGHT + (m_ajustPosition.y - Tile::TILE_CENTER_Y) * 2
				 , Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				 , Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
}

// 位置を列に調整する関数
void Player::AjustCloumn()
{
	// 調整する必要はなし
	if (m_ajustPosition.x == Tile::TILE_CENTER_X) return;

	// 左右にずれている場合は調整する
	if (m_ajustPosition.x < Tile::TILE_CENTER_X)
	{
		m_ajustPosition.x++;
	}
	else
	{
		m_ajustPosition.x--;
	}
}

// 位置を行に調整する関数
void Player::AjustRow()
{
	// 調整する必要はなし
	if (m_ajustPosition.y == Tile::TILE_CENTER_Y) return;

	// 上下にずれている場合は調整する
	if (m_ajustPosition.y < Tile::TILE_CENTER_Y)
	{
		m_ajustPosition.y++;
	}
	else
	{
		m_ajustPosition.y--;
	}
}

// 移動可能か調べる関数
bool Player::IsMovable(Stage* pStage) const
{
	// ハシゴなら移動可能
	if (pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Ladder) return true;

	// ロープにつかんでいれば移動可能
	if ( (pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Rope)
	  && (m_ajustPosition.y == Tile::TILE_CENTER_Y)
	   )
	{
		return true;
	}

	// 空中に浮いている
	if (m_ajustPosition.y < Tile::TILE_CENTER_Y) return false;

	// 一番下なら移動可能
	if (m_tilePosition.y == Stage::STAGE_HEIGHT - 1) return true;

	// 下の行が落下可能なら
	if (Tile::IsMovableTileFall(pStage->GetTileType(m_tilePosition.x, m_tilePosition.y + 1))) return false;

	return true;
}

// 上に移動可能か調べる関数
bool Player::IsMovableUp(Stage* pStage) const
{
	// ハシゴがある？
	if (pStage->GetTileType(m_tilePosition.x, m_tilePosition.y) == Tile::TileType::Ladder)
	{
		// ハシゴ上なのでハシゴの真ん中までは移動可能
		if (m_ajustPosition.y > Tile::TILE_CENTER_Y) return true;

		// 一番上なら移動不可
		if (m_tilePosition.y == 0) return false;

		// 上の行が移動可能なら
		if (Tile::IsMovableTileULR(pStage->GetTileType(m_tilePosition.x, m_tilePosition.y - 1))) return true;
	}
	else
	{
		if (m_ajustPosition.y > Tile::TILE_CENTER_Y) return true;
	}
	return false;
}

bool Player::IsMovableDown(Stage* pStage) const
{
	// 少し上にいるので下に移動可能
	if (m_ajustPosition.y < Tile::TILE_CENTER_Y) return true;

	// 一番下なら移動不可
	if (m_tilePosition.y == Stage::STAGE_HEIGHT - 1) return false;

	// 下の行が移動可能なら
	if (Tile::IsMovableTileDown(pStage->GetTileType(m_tilePosition.x, m_tilePosition.y + 1))) return true;

	return false;
}

// 左に移動可能か調べる関数
bool Player::IsMovableLeft(Stage* pStage) const
{
	// まだ左方向に移動させる余地がある
	if (m_ajustPosition.x > Tile::TILE_CENTER_X) return true;

	// 左隅なので移動不可
	if (m_tilePosition.x == 0) return false;

	// 移動先タイルが移動可能か調べる
	return Tile::IsMovableTileULR(pStage->GetTileType(m_tilePosition.x - 1, m_tilePosition.y));
}

// 右に移動可能か調べる関数
bool Player::IsMovableRight(Stage* pStage) const
{
	// まだ右方向に移動させる余地がある
	if (m_ajustPosition.x < Tile::TILE_CENTER_X) return true;

	// 右隅なので移動不可
	if (m_tilePosition.x == Stage::STAGE_WIDTH - 1) return false;

	// 移動先タイルが移動可能か調べる
	return Tile::IsMovableTileULR(pStage->GetTileType(m_tilePosition.x + 1, m_tilePosition.y));
}

// アニメーションステートの設定
void Player::SetAnimationState(AnimationState beginAnimState, AnimationState endAnimState)
{
	int begin = static_cast<int>(beginAnimState);
	int end = static_cast<int>(endAnimState);
	int animState = static_cast<int>(m_animationState);

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

	m_animationState = static_cast<AnimationState>(animState);
}
