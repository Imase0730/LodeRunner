//--------------------------------------------------------------------------------------
// File: Player.cpp
//
//--------------------------------------------------------------------------------------
#include "Player.h"
#include "Game/Tile.h"

// コンストラクタ
Player::Player()
	: m_isActive{ false }
	, m_position{ 0, 0 }
	, m_faceDirection{ FaceDirection::Right }
	, m_animationState{ AnimationState::Run01_R }
{
}

// デストラクタ
Player::~Player()
{
}

// 初期化処理
void Player::Initialize(POINT position)
{
	// アクティブにする
	m_isActive = true;

	// 位置を初期化する
	m_position = position;

	// プレイヤーの向き
	m_faceDirection = FaceDirection::Right;
}

// 更新処理
void Player::Update(int keyCondition, int keyTrigger, Stage* pStage)
{
	// 上キーが押された
	if (keyCondition & PAD_INPUT_UP)
	{
		// 上に移動可能か？
		if (IsMovableUp(pStage))
		{
			// 列に合わせるようにX座標を調整する
			AjustCloumn();

			// 上に移動する
			m_position.y -= 2;

			// ハシゴを登るアニメーション
			SetAnimationState(AnimationState::Climb01, AnimationState::Climb02);
		}
	}
	// 下キーが押された
	if (keyCondition & PAD_INPUT_DOWN)
	{

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
			m_position.x -= 2;

			// プレイヤーの位置のタイルを取得
			int column = m_position.x / Tile::TILE_WIDTH;
			int row = m_position.y / Tile::TILE_HEIGHT;
	
			// ロープの場合
			if (pStage->GetTileType(column, row) == Tile::TileType::Rope)
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
			m_position.x += 2;

			// プレイヤーの位置のタイルを取得
			int column = m_position.x / Tile::TILE_WIDTH;
			int row = m_position.y / Tile::TILE_HEIGHT;

			// ロープの場合
			if (pStage->GetTileType(column, row) == Tile::TileType::Rope)
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
	DrawRectGraph(m_position.x - Tile::CENTER_OFFSET_X, m_position.y - Tile::CENTER_OFFSET_Y
		, Tile::TILE_WIDTH * pos.x, Tile::TILE_HEIGHT * pos.y
		, Tile::TILE_WIDTH, Tile::TILE_HEIGHT, ghTileset, TRUE);
}

// 位置を列に調整する関数
void Player::AjustCloumn()
{
	int cloumn = m_position.x / Tile::TILE_WIDTH;
	int x = m_position.x % Tile::TILE_WIDTH;

	// 調整する必要はなし
	if (x == Tile::CENTER_OFFSET_X) return;

	// 左右にずれている場合は調整する
	if (x < Tile::CENTER_OFFSET_X)
	{
		x += 2;
		if (x > Tile::CENTER_OFFSET_X) x = Tile::CENTER_OFFSET_X;
	}
	else
	{
		x -= 2;
		if (x < Tile::CENTER_OFFSET_X) x = Tile::CENTER_OFFSET_X;
	}
	m_position.x = cloumn * Tile::TILE_WIDTH + x;
}

// 位置を行に調整する関数
void Player::AjustRow()
{
	int row = m_position.y / Tile::TILE_HEIGHT;
	int y = m_position.y % Tile::TILE_HEIGHT;

	// 調整する必要はなし
	if (y == Tile::CENTER_OFFSET_Y) return;

	// 上下にずれている場合は調整する
	if (y < Tile::CENTER_OFFSET_Y)
	{
		y += 2;
		if (y > Tile::CENTER_OFFSET_Y) y = Tile::CENTER_OFFSET_Y;
	}
	else
	{
		y -= 2;
		if (y < Tile::CENTER_OFFSET_Y) y = Tile::CENTER_OFFSET_Y;
	}
	m_position.y = row * Tile::TILE_HEIGHT + y;
}

// 上に移動可能か調べる関数
bool Player::IsMovableUp(Stage* pStage) const
{
	// プレイヤーの位置のタイルを取得
	int column = m_position.x / Tile::TILE_WIDTH;
	int row = m_position.y / Tile::TILE_HEIGHT;
	int y = m_position.y % Tile::TILE_HEIGHT;

	// ハシゴがある？
	if (pStage->GetTileType(column, row) == Tile::TileType::Ladder)
	{
		// ハシゴ上なのでハシゴの真ん中までは移動可能
		if (y > Tile::CENTER_OFFSET_Y) return true;

		// 一番上なら移動不可
		if (row == 0) return false;

		// 上の行が移動可能なら
		if (Tile::IsMovableTile(pStage->GetTileType(column, row - 1))) return true;
	}
	// 足元がハシゴの場合は上まで登ることが可能
	else if (pStage->GetTileType(column, row + 1) == Tile::TileType::Ladder)
	{
		if (y > Tile::CENTER_OFFSET_Y) return true;
	}
	return false;
}

// 左に移動可能か調べる関数
bool Player::IsMovableLeft(Stage* pStage) const
{
	// まだ左方向に移動させる余地がある
	if ((m_position.x % Tile::TILE_WIDTH) > Tile::CENTER_OFFSET_X) return true;

	int column = m_position.x / Tile::TILE_WIDTH;
	int row = m_position.y / Tile::TILE_HEIGHT;

	// 左隅なので移動不可
	if (column == 0) return false;

	column--;

	// 移動先タイルが移動可能か調べる
	return Tile::IsMovableTile(pStage->GetTileType(column, row));
}

// 右に移動可能か調べる関数
bool Player::IsMovableRight(Stage* pStage) const
{
	// まだ右方向に移動させる余地がある
	if ((m_position.x % Tile::TILE_WIDTH) < Tile::CENTER_OFFSET_X) return true;

	int column = m_position.x / Tile::TILE_WIDTH;
	int row = m_position.y / Tile::TILE_HEIGHT;

	// 右隅なので移動不可
	if (column == Stage::STAGE_WIDTH - 1) return false;

	column++;

	// 移動先タイルが移動可能か調べる
	return Tile::IsMovableTile(pStage->GetTileType(column, row));
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
