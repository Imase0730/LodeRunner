//--------------------------------------------------------------------------------------
// File: Player.cpp
//
//--------------------------------------------------------------------------------------
#include "Player.h"
#include "GamePlayScene.h"
#include "Game/Level.h"

// コンストラクタ
Player::Player(GamePlayScene* pScene, Level* pLevel)
	: m_pGamePlayScene{ pScene }
	, m_pLevel{ pLevel }
	, m_isActive{ false }
	, m_isAlive{ false }
	, m_isVisible{ false }
	, m_tilePosition{ 0, 0 }
	, m_adjustPosition{ 0, 0 }
	, m_faceDirection{ Direction::Right }
	, m_playerAnimationState{ PlayerAnimationState::Run01_R }
	, m_digDirection{ DigDirection::NotDigging }
	, m_digAnimationState{ DigAnimationState::NotDigging }
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

	// 生存フラグ初期化
	m_isAlive = true;

	// 表示(ON)
	m_isVisible = true;

	// 位置を初期化する
	m_tilePosition = tilePosition;
	m_adjustPosition = ajustPosition;

	// Page1にプレイヤーの位置を設定する
	m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Player);

	// 各変数の初期化
	m_faceDirection = Direction::Right;
	m_playerAnimationState = PlayerAnimationState::Run01_R;
	m_digDirection = DigDirection::NotDigging;
	m_digAnimationState = DigAnimationState::NotDigging;
}

// 更新処理
void Player::Update(int keyCondition, int keyTrigger)
{
	// 死んでいるなら何もしない
	if (!m_isAlive) return;

	// 左向きに掘っている
	if (m_digDirection == DigDirection::Left)
	{
		// アニメーションを更新する
		UpdateDigAnimation();
		// 左に掘る
		DigLeft();
		return;
	}

	// 右向きに掘っている
	if (m_digDirection == DigDirection::Right)
	{
		// アニメーションを更新する
		UpdateDigAnimation();
		// 右に掘る
		DigRight();
		return;
	}

	// 落下中か
	if (IsFalling())
	{
		// 落下中
		Falling();
		return;
	}

	// 上キーが押された
	if (keyCondition & PAD_INPUT_UP)
	{
		// 上に移動
		MoveUp();
		return;
	}

	// 下キーが押された
	if (keyCondition & PAD_INPUT_DOWN)
	{
		// 下に移動
		MoveDown();
		return;
	}

	// 掘っていないなら
	if (m_digDirection == DigDirection::NotDigging)
	{
		// Zキーが押された左側を掘る
		if (keyTrigger & PAD_INPUT_1)
		{
			// 左方向に掘る
			m_digDirection = DigDirection::Left;
			// 掘りアニメーション設定
			m_digAnimationState = DigAnimationState::Dig01;
			// 左に掘る
			DigLeft();
			return;
		}

		// Xキーが押された右側を掘る
		if (keyTrigger & PAD_INPUT_2)
		{
			// 右方向に掘る
			m_digDirection = DigDirection::Right;
			// 掘りアニメーション設定
			m_digAnimationState = DigAnimationState::Dig01;
			// 右に掘る
			DigRight();
			return;
		}
	}

	if ((keyTrigger & PAD_INPUT_1) && (m_digDirection == DigDirection::NotDigging))
	{
		// 左方向に掘る
		m_digDirection = DigDirection::Left;
		// 掘りアニメーション設定
		m_digAnimationState = DigAnimationState::Dig01;
		// 左に掘る
		DigLeft();
		return;
	}

	// Xキーが押されたかつ掘っていないなら右側を掘る
	if ((keyTrigger & PAD_INPUT_2) && (m_digDirection == DigDirection::NotDigging))
	{
		// 右方向に掘る
		m_digDirection = DigDirection::Right;
		// 掘りアニメーション設定
		m_digAnimationState = DigAnimationState::Dig01;
		// 右に掘る
		DigRight();
		return;
	}

	// 左キーが押された
	if (keyCondition & PAD_INPUT_LEFT)
	{
		// 左に移動
		MoveLeft();
		return;
	}

	// 右キーが押された
	if (keyCondition & PAD_INPUT_RIGHT)
	{
		// 右に移動
		MoveRight();
		return;
	}
}

// 描画処理
void Player::Render(int ghTileset) const
{
	// 表示(ON)
	if (m_isVisible)
	{
		// プレイヤーの描画
		POINT pos = PLAYER_SPRITES[static_cast<int>(m_playerAnimationState)];
		DrawRectGraph(m_tilePosition.x * Tile::TILE_PIXEL_WIDTH + (m_adjustPosition.x - Tile::TILE_CENTER_X) * 2
			, m_tilePosition.y * Tile::TILE_PIXEL_HEIGHT + (m_adjustPosition.y - Tile::TILE_CENTER_Y) * 2
			, Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
			, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
	}

	// 掘っている時の破片の描画
	if (m_digAnimationState != DigAnimationState::NotDigging)
	{
		const POINT* SPRITES = nullptr;
		int x = 0;

		// 掘っている向きに応じて絵を変える
		if (m_digDirection == DigDirection::Left)
		{
			x = -1;
			SPRITES = DIG_DEBRIS_LEFT_SPRITES;
		}
		else
		{
			x = 1;
			SPRITES = DIG_DEBRIS_RIGHT_SPRITES;
		}

		POINT pos = SPRITES[static_cast<int>(m_digAnimationState)];
		// 表示する破片があるなら
		if (pos.x >= 0)
		{
			// 掘っている時の破片の描画
			DrawRectGraph((m_tilePosition.x + x) * Tile::TILE_PIXEL_WIDTH, m_tilePosition.y * Tile::TILE_PIXEL_HEIGHT
				, Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
		}

		// 掘っている最中のレンガなら
		if (m_digAnimationState != DigAnimationState::NotDigging)
		{
			// 掘っている最中のレンガの描画
			POINT pos = DIG_BRICK_SPRITES[static_cast<int>(m_digAnimationState)];

			// タイルの描画
			DrawRectGraph((m_tilePosition.x + x) * Tile::TILE_PIXEL_WIDTH, (m_tilePosition.y + 1) * Tile::TILE_PIXEL_HEIGHT
				, Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				, Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, FALSE);
		}

	}
}

// 位置を列に調整する関数
void Player::AjustCloumn()
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
void Player::AjustRow()
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
bool Player::IsFalling()
{
	// ハシゴなら移動可能
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Ladder) return false;

	// ロープにつかんでいれば移動可能
	if ( (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Rope)
	  && (m_adjustPosition.y == Tile::TILE_CENTER_Y)
	   )
	{
		return false;
	}

	// 空中に浮いている
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y) return true;

	// 一番下なら移動可能
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// 下の行が落下可能なら
	Tile::Type page1 = m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y + 1);
	Tile::Type page2 = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y + 1);
	if (IsMovableTileFall(page1, page2)) return true;

	return false;
}

// 上に移動可能か調べる関数
bool Player::IsMovableUp()
{
	// ハシゴがある？
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Ladder)
	{
		// ハシゴ上なのでハシゴの真ん中までは移動可能
		if (m_adjustPosition.y > Tile::TILE_CENTER_Y) return true;

		// 一番上なら移動不可
		if (m_tilePosition.y == 0) return false;

		// 上の行が移動可能なら
		if (IsMovableTileULR(m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y - 1))) return true;
	}
	else
	{
		if (m_adjustPosition.y > Tile::TILE_CENTER_Y) return true;
	}
	return false;
}

bool Player::IsMovableDown()
{
	// 少し上にいるので下に移動可能
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y) return true;

	// 一番下なら移動不可
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// 下の行が移動可能なら
	if (IsMovableTileDown(m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y + 1))) return true;

	return false;
}

// 左に掘れるか調べる関数
bool Player::IsDiggableLeft()
{
	// 一番下の行なら掘れない
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// 一番左の列なら掘れない
	if (m_tilePosition.x == 0) return false;

	// 左下がブロックでない
	if (m_pLevel->GetTilePage1(m_tilePosition.x - 1, m_tilePosition.y + 1) != Tile::Type::Blick) return false;

	// 左が空白
	if (m_pLevel->GetTilePage1(m_tilePosition.x - 1, m_tilePosition.y) != Tile::Type::Empty) return false;

	return true;
}

// 右に掘れるか調べる関数
bool Player::IsDiggableRight()
{
	// 一番下の行なら掘れない
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// 一番右の列なら掘れない
	if (m_tilePosition.x == Level::MAX_GAME_COLMUN) return false;

	// 右下がブロックでない
	if (m_pLevel->GetTilePage1(m_tilePosition.x + 1, m_tilePosition.y + 1) != Tile::Type::Blick) return false;

	// 右が空白
	if (m_pLevel->GetTilePage1(m_tilePosition.x + 1, m_tilePosition.y) != Tile::Type::Empty) return false;

	return true;
}

// 左に移動可能か調べる関数
bool Player::IsMovableLeft()
{
	// まだ左方向に移動させる余地がある
	if (m_adjustPosition.x > Tile::TILE_CENTER_X) return true;

	// 左隅なので移動不可
	if (m_tilePosition.x == 0) return false;

	// 移動先タイルが移動可能か調べる
	return IsMovableTileULR(m_pLevel->GetTilePage1(m_tilePosition.x - 1, m_tilePosition.y));
}

// 右に移動可能か調べる関数
bool Player::IsMovableRight()
{
	// まだ右方向に移動させる余地がある
	if (m_adjustPosition.x < Tile::TILE_CENTER_X) return true;

	// 右隅なので移動不可
	if (m_tilePosition.x == Level::MAX_GAME_COLMUN) return false;

	// 移動先タイルが移動可能か調べる
	return IsMovableTileULR(m_pLevel->GetTilePage1(m_tilePosition.x + 1, m_tilePosition.y));
}

// 左に掘る
void Player::DigLeft()
{
	// 左下が掘ることが可能か？
	if (IsDiggableLeft())
	{
		// 列に合わせるようにX座標を調整する
		AjustCloumn();
		// 行に合わせるようにY座標を調整する
		AjustRow();

		// プレイヤーのアニメーションを設定（掘っている途中で絵を変えている）
		if (m_digAnimationState == DigAnimationState::Dig01) m_playerAnimationState = PlayerAnimationState::Dig_L;
		if (m_digAnimationState == DigAnimationState::Dig06) m_playerAnimationState = PlayerAnimationState::Run01_L;

		// 掘り終わった？
		if (m_digAnimationState == DigAnimationState::NotDigging)
		{
			// 掘り終えた
			m_digDirection = DigDirection::NotDigging;
			// 掘った場所を空白に設定
			m_pLevel->SetTilePage1(m_tilePosition.x - 1, m_tilePosition.y + 1, Tile::Type::Empty);
			// 掘った場所のレンガの復元設定
			m_pGamePlayScene->SetFillBrick(m_tilePosition.x - 1, m_tilePosition.y + 1);
		}
	}
	else
	{
		// 掘る事をキャンセル
		m_digDirection = DigDirection::NotDigging;
		m_digAnimationState = DigAnimationState::NotDigging;
	}
}

// 右に掘る
void Player::DigRight()
{
	// 右下が掘ることが可能か？
	if (IsDiggableRight())
	{
		// 列に合わせるようにX座標を調整する
		AjustCloumn();
		// 行に合わせるようにY座標を調整する
		AjustRow();

		// プレイヤーのアニメーションを設定（掘っている途中で絵を変えている）
		if (m_digAnimationState == DigAnimationState::Dig01) m_playerAnimationState = PlayerAnimationState::Dig_R;
		if (m_digAnimationState == DigAnimationState::Dig06) m_playerAnimationState = PlayerAnimationState::Run01_R;

		// 掘り終わった？
		if (m_digAnimationState == DigAnimationState::NotDigging)
		{
			// 掘り終えた
			m_digDirection = DigDirection::NotDigging;
			// 掘った場所を空白に設定
			m_pLevel->SetTilePage1(m_tilePosition.x + 1, m_tilePosition.y + 1, Tile::Type::Empty);
			// 掘った場所のレンガの復元設定
			m_pGamePlayScene->SetFillBrick(m_tilePosition.x + 1, m_tilePosition.y + 1);
		}
	}
	else
	{
		// 掘る事をキャンセル
		m_digDirection = DigDirection::NotDigging;
		m_digAnimationState = DigAnimationState::NotDigging;
	}
}

// 落下中
void Player::Falling()
{
	// 列に合わせるようにX座標を調整する
	AjustCloumn();

	// 落下
	m_adjustPosition.y++;
	if (m_adjustPosition.y > 4)
	{
		m_adjustPosition.y = 0;
		// Page1を元に戻す
		m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
		// Page1に書き込む
		m_tilePosition.y++;
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Player);
	}

	// 金塊が拾えるかチェック
	CheckGoldPickedUp();

	// 落下アニメーション
	if (m_faceDirection == Direction::Left)
	{
		m_playerAnimationState = PlayerAnimationState::Fall_L;
	}
	else
	{
		m_playerAnimationState = PlayerAnimationState::Fall_R;
	}
}

// 上に移動
void Player::MoveUp()
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
			// Page1を元に戻す
			m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
			// Page1に書き込む
			m_tilePosition.y--;
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Player);
		}

		// 金塊が拾えるかチェック
		CheckGoldPickedUp();

		// ハシゴを登るアニメーション
		SetPlayerAnimationState(PlayerAnimationState::Climb01, PlayerAnimationState::Climb02);
	}
}

// 下に移動
void Player::MoveDown()
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
			// Page1を元に戻す
			m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
			// Page1に書き込む
			m_tilePosition.y++;
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Player);
		}

		// 金塊が拾えるかチェック
		CheckGoldPickedUp();

		// ハシゴを登るアニメーション
		SetPlayerAnimationState(PlayerAnimationState::Climb01, PlayerAnimationState::Climb02);
	}
}

// 左に移動
void Player::MoveLeft()
{
	// 左に移動可能か？
	if (IsMovableLeft())
	{
		// 行に合わせるようにY座標を調整する
		AjustRow();

		// 左向きに
		m_faceDirection = Direction::Left;

		// 左に移動する
		m_adjustPosition.x--;
		if (m_adjustPosition.x < 0)
		{
			m_adjustPosition.x = 4;
			// Page1を元に戻す
			m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
			// Page1に書き込む
			m_tilePosition.x--;
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Player);
		}

		// 金塊が拾えるかチェック
		CheckGoldPickedUp();

		// ロープの場合
		if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Rope)
		{
			// ロープで移動するアニメーション
			SetPlayerAnimationState(PlayerAnimationState::Rope01_L, PlayerAnimationState::Rope03_L);
		}
		else
		{
			// 走るアニメーション
			SetPlayerAnimationState(PlayerAnimationState::Run01_L, PlayerAnimationState::Run03_L);
		}
	}
}

// 右に移動
void Player::MoveRight()
{
	// 右に移動可能か？
	if (IsMovableRight())
	{
		// 行に合わせるようにY座標を調整する
		AjustRow();

		// 右向きに
		m_faceDirection = Direction::Right;

		// 右移動する
		m_adjustPosition.x++;
		if (m_adjustPosition.x > 4)
		{
			m_adjustPosition.x = 0;
			// Page1を元に戻す
			m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
			// Page1に書き込む
			m_tilePosition.x++;
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Player);
		}

		// 金塊が拾えるかチェック
		CheckGoldPickedUp();

		// ロープの場合
		if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Rope)
		{
			// ロープで移動するアニメーション
			SetPlayerAnimationState(PlayerAnimationState::Rope01_R, PlayerAnimationState::Rope03_R);
		}
		else
		{
			// 走るアニメーション
			SetPlayerAnimationState(PlayerAnimationState::Run01_R, PlayerAnimationState::Run03_R);
		}
	}
}

// プレイヤーアニメーションステートの設定
void Player::SetPlayerAnimationState(PlayerAnimationState beginAnimState, PlayerAnimationState endAnimState)
{
	int begin = static_cast<int>(beginAnimState);
	int end = static_cast<int>(endAnimState);
	int animState = static_cast<int>(m_playerAnimationState);

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

	m_playerAnimationState = static_cast<PlayerAnimationState>(animState);
}

// 掘るアニメーションステートの設定
void Player::UpdateDigAnimation()
{
	if (static_cast<int>(m_digAnimationState) == static_cast<int>(DigAnimationState::Dig12))
	{
		m_digAnimationState = DigAnimationState::NotDigging;
	}
	else
	{
		m_digAnimationState = static_cast<DigAnimationState>(static_cast<int>(m_digAnimationState) + 1);
	}
}

// 金塊が拾えるか調べる関数
void Player::CheckGoldPickedUp()
{
	// タイルの中心でなければチェックしない
	if (m_adjustPosition.x != Tile::TILE_CENTER_X) return;
	if (m_adjustPosition.y != Tile::TILE_CENTER_Y) return;

	// 金塊があれば金塊を拾う
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Gold)
	{
		// 空白にする
		m_pLevel->SetTilePage2(m_tilePosition.x, m_tilePosition.y, Tile::Type::Empty);
		// ステージ上の金塊の数を減らす
		m_pLevel->LostGold();
		// 得点を加算（２５０点）
		m_pGamePlayScene->AddScore(GamePlayScene::GOLD_SCORE);
	}
}

bool Player::IsMovableTileFall(Tile::Type page1, Tile::Type page2)
{
	// 空白なら落下
	if (page1 == Tile::Type::Empty)
	{
		return true;
	}

	// ガードなら落下しない
	if (page1 == Tile::Type::Guard)
	{
		return false;
	}

	// レンガ、石、ハシゴなら落下しない
	if ((page2 == Tile::Type::Blick) || (page2 == Tile::Type::Stone) || (page2 == Tile::Type::Ladder))
	{
		return false;
	}

	return true;
}

// 移動可能なタイルか調べる関数（上左右）
bool Player::IsMovableTileULR(Tile::Type tile)
{
	if ( (tile == Tile::Type::Blick)	// page1:レンガ
	  || (tile == Tile::Type::Stone)	// page1:石
	  || (tile == Tile::Type::Trap)	// page1:罠
	   )
	{
		// 移動不可
		return false;
	}
	// 移動可能
	return true;
}

// 移動可能なタイルか調べる関数（下）
bool Player::IsMovableTileDown(Tile::Type tile)
{
	if ( (tile == Tile::Type::Blick)	// page1:レンガ
	  || (tile == Tile::Type::Stone)	// page1:石
	   )
	{
		// 移動不可
		return false;
	}
	// 移動可能
	return true;
}

