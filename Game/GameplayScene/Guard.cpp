//--------------------------------------------------------------------------------------
// File: Guard.cpp
//
//--------------------------------------------------------------------------------------
#include "Guard.h"
#include "GamePlayScene.h"
#include "Game/Level.h"
#include "Player.h"

// コンストラクタ
Guard::Guard(GamePlayScene* pScene, Level* pLevel)
	: m_pGamePlayScene{ pScene }
	, m_pLevel{ pLevel }
	, m_isActive{ false }
	, m_isAlive{ false }
	, m_isDisplay{ false }
	, m_tilePosition{ 0, 0 }
	, m_adjustPosition{ 0, 0 }
	, m_faceDirection{ Direction::Left }
	, m_guardAnimationState{ GuardAnimationState::Run01_L }
	, m_actionStateTimer{ 0 }
	, m_resurrectionTimer{ 0 }
{
}

// デストラクタ
Guard::~Guard()
{
}

// 初期化処理
void Guard::Initialize(POINT tilePosition, POINT ajustPosition)
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

	// Page1にガードの位置を設定する
	m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Guard);

	// 各変数の初期化
	m_faceDirection = Direction::Left;
	m_guardAnimationState = GuardAnimationState::Run01_L;
	m_actionStateTimer = 0;
	m_resurrectionTimer = 0;
}

// 更新処理
void Guard::Update()
{
	// 穴に落ちた時の処理
	if (HandleFallingInHole()) return;

	// 落下処理
	if (HandlFalling()) return;

	// 移動処理
	HandleMovement();
}

// 穴に落ちた時の処理
bool Guard::HandleFallingInHole()
{
	// 穴の中？
	if (IsInHole())
	{
		// 穴に落ちているタイマーを減算
		m_actionStateTimer--;

		// 穴の中なら落ちないようにタイマーを０にしないようにする
		if ( (m_actionStateTimer == 0)
		  && (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Blick)
		   )
		{
			m_actionStateTimer++;
		}

		// 穴に落ちた直後、数フレームはアニメーションしない
		if (m_actionStateTimer > HOLE_ANIMATION_START_FRAME) return true;

		// 横に揺れるアニメーション
		int idx = m_actionStateTimer - (HOLE_ANIMATION_START_FRAME - HOLE_ANIMATION_TABLE_SIZE + 1);
		if (idx >= 0)
		{
			m_adjustPosition.x = HOLE_ANIMATION_TABLE[idx];
			return true;
		}
	}
	return false;
}

// 落下処理
bool Guard::HandlFalling()
{
	// 穴に落ちている最中は落下せず、這い上がるため落ちないようにする
	if (!IsInHole())
	{
		// 落下中か
		if (IsFalling())
		{
			// 落下中
			Falling();
			return true;
		}
	}
	return false;
}

// 移動処理
void Guard::HandleMovement()
{
	// 移動方向を決める
	switch (DecideMoveDirection())
	{
	case Guard::MoveDirection::UP:
		MoveUp();
		break;
	case Guard::MoveDirection::Down:
		MoveDown();
		break;
	case Guard::MoveDirection::Left:
		MoveLeft();
		break;
	case Guard::MoveDirection::Right:
		MoveRight();
		break;
	default:
		break;
	}
}

// 穴の中か調べる関数
bool Guard::IsInHole() const
{
	if (m_actionStateTimer > 0) return true;
	return false;
}

// 描画処理
void Guard::Render(int ghTileset)
{
	// 表示(OFF)
	if (!m_isDisplay) return;

	// 復活中
	if (m_resurrectionTimer > 0)
	{
		// 復活のアニメーション
		if (m_resurrectionTimer == RESURRECTION_ANIME_TIME_EGG01) m_guardAnimationState = GuardAnimationState::Egg01;
		if (m_resurrectionTimer == RESURRECTION_ANIME_TIME_EGG02) m_guardAnimationState = GuardAnimationState::Egg02;
	}

	// ガードの描画
	POINT pos = GUARD_SPRITES[static_cast<int>(m_guardAnimationState)];
	DrawRectGraph( m_tilePosition.x * Tile::TILE_PIXEL_WIDTH + (m_adjustPosition.x - Tile::TILE_CENTER_X) * 2
				 , m_tilePosition.y * Tile::TILE_PIXEL_HEIGHT + (m_adjustPosition.y - Tile::TILE_CENTER_Y) * 2
				 , Tile::TILE_PIXEL_WIDTH * pos.x, Tile::TILE_PIXEL_HEIGHT * pos.y
				 , Tile::TILE_PIXEL_WIDTH, Tile::TILE_PIXEL_HEIGHT, ghTileset, TRUE);
}

// 指定位置から復活する関数
void Guard::Resurrection(int colmun, int row)
{
	// アニメーションを初期化
	m_guardAnimationState = GuardAnimationState::AnimNone;
	// 位置を設定
	SetTilePosition(colmun, row);
	// 復活タイマーを設定
	SetResurrectionTimer(RESURRECTION_ANIME_TIME_NONE);
}

// 位置を列に調整する関数
void Guard::AjustCloumn()
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
void Guard::AjustRow()
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
bool Guard::IsFalling()
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
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y)
	{
		return true;
	}

	// 一番下なら移動可能
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// 下の行が落下可能なら
	Tile::Type page1 = m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y + 1);
	Tile::Type page2 = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y + 1);
	if (IsMovableTileFall(page1, page2)) return true;

	return false;
}

// 上に移動可能か調べる関数
bool Guard::IsMovableUp()
{
	// ハシゴ上なのでハシゴの真ん中までは移動可能
	if (m_adjustPosition.y > Tile::TILE_CENTER_Y) return true;

	// 一番上なら移動不可
	if (m_tilePosition.y == 0) return false;

	// 上の行が移動可能なら
	Tile::Type page1 = m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y - 1);
	if (IsMovableTileULR(page1)) return true;

	return false;
}

bool Guard::IsMovableDown()
{
	// 少し上にいるので下に移動可能
	if (m_adjustPosition.y < Tile::TILE_CENTER_Y) return true;

	// 一番下なら移動不可
	if (m_tilePosition.y == Level::MAX_GAME_ROW) return false;

	// 下の行が移動可能なら
	Tile::Type page1 = m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y + 1);
	if (IsMovableTileD(page1)) return true;

	return false;
}

// 左に移動可能か調べる関数
bool Guard::IsMovableLeft()
{
	// まだ左方向に移動させる余地がある
	if (m_adjustPosition.x > Tile::TILE_CENTER_X) return true;

	// 左隅なので移動不可
	if (m_tilePosition.x == 0) return false;

	// 移動先タイルが移動可能か調べる
	Tile::Type page1 = m_pLevel->GetTilePage1(m_tilePosition.x - 1, m_tilePosition.y);
	if (IsMovableTileULR(page1)) return true;

	return false;
}

// 右に移動可能か調べる関数
bool Guard::IsMovableRight()
{
	// まだ右方向に移動させる余地がある
	if (m_adjustPosition.x < Tile::TILE_CENTER_X) return true;

	// 右隅なので移動不可
	if (m_tilePosition.x == Level::MAX_GAME_COLMUN) return false;

	// 移動先タイルが移動可能か調べる
	Tile::Type page1 = m_pLevel->GetTilePage1(m_tilePosition.x + 1, m_tilePosition.y);
	if (IsMovableTileULR(page1)) return true;

	return false;
}

// 落下中
void Guard::Falling()
{
	// 列に合わせるようにX座標を調整する
	AjustCloumn();

	// 落下
	m_adjustPosition.y++;
	if (m_adjustPosition.y > 4)
	{
		// 下のタイルへ移動したのでPage1のタイル情報を書き換える
		m_adjustPosition.y = 0;

		Tile::Type page2 = m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y);
		if (page2 != Tile::Type::Blick)
		{
			// Page2→Page1へコピー
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, page2);
		}
		else
		{
			// 掘った穴を通過
			m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Empty);
		}

		// 下の行へ移動
		m_tilePosition.y++;

		// プレイヤーなら死亡
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}

		// 掘った穴なら
		if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Blick)
		{
			// 金塊を持っている
			if (m_actionStateTimer < 0)
			{
				// 金塊を落とす
				m_actionStateTimer = 0;
				// 金塊が置けるか？
				if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y - 1) == Tile::Type::Empty)
				{
					// 金塊を上に置く
					m_pLevel->SetTilePage2(m_tilePosition.x, m_tilePosition.y - 1, Tile::Type::Gold);
				}
				else
				{
					// 金塊をロストする
					m_pLevel->LostGold();
				}
			}
		}

		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Guard);
	}
	else
	{
		// 落下中のタイルチェックをする
		if (m_adjustPosition.y == Tile::TILE_CENTER_Y)
		{
			// 金塊が拾えるかチェック
			CheckGoldPickedUp();

			// 掘った穴なら
			if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Blick)
			{
				// 金塊を持っていればロストする
				if (m_actionStateTimer < 0) m_pLevel->LostGold();

				// 行動タイマー設定（復活用に使用する）
				m_actionStateTimer = RESURRECTION_TIME;

				// 得点を加算（７５点）
				m_pGamePlayScene->AddScore(GamePlayScene::GUARD_HOLE_SCORE);

				// 効果音を鳴らす（穴に落ちる音）
				m_pGamePlayScene->GetSound().PlaySound(Sound::SoundID::InHole);
			}
		}
	}

	// 落下アニメーション
	if (m_faceDirection == Direction::Left)
	{
		m_guardAnimationState = GuardAnimationState::Fall_L;
	}
	else
	{
		m_guardAnimationState = GuardAnimationState::Fall_R;
	}
}

// 移動方向を決める関数
Guard::MoveDirection Guard::DecideMoveDirection()
{
	// 穴の中
	if ( (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Blick)
	  && (m_actionStateTimer > 0)	// 穴に落ちている
	   )
	{
		// 上へ
		return MoveDirection::UP;
	}

	// プレイヤーの位置を取得
	Player* pPlayer = m_pGamePlayScene->GetPlayer();
	int playerColumn = pPlayer->GetTilePosition().x;
	int playerRow = pPlayer->GetTilePosition().y;

	// プレイヤーと同じ行にいる
	if (m_tilePosition.y == playerRow)
	{
		// プレイヤーの位置まで移動可能か？
		if (IsMovableColumn(playerColumn))
		{
			// 左右どちらにプレイヤーがいるかチェックする
			if (m_tilePosition.x > playerColumn)
			{
				// 左に移動
				return MoveDirection::Left;
			}
			else
			{
				// 右に移動
				return MoveDirection::Right;
			}
		}
	}

	// 左右の位置調べる位置を取得
	int leftColmunLimit = 0;
	int rightColmunLimit = 0;
	GetLeftRightLimits(&leftColmunLimit, Direction::Left, 0);
	GetLeftRightLimits(&rightColmunLimit, Direction::Right, Level::MAX_GAME_COLMUN);

	// プレイヤーとガードとの疑似的な距離をリセット
	int bestGuardDistance = 255;

	// どの方向へ移動すべきか決める
	MoveDirection moveDirection = MoveDirection::None;
	MoveDirection dir{};
	dir = SelectMoveUpAndDown(&bestGuardDistance);
	if (dir != MoveDirection::None) moveDirection = dir;
	dir = SelectMoveLeftAndRight(&bestGuardDistance, leftColmunLimit, Direction::Left);
	if (dir != MoveDirection::None) moveDirection = dir;
	dir = SelectMoveLeftAndRight(&bestGuardDistance, rightColmunLimit, Direction::Right);
	if (dir != MoveDirection::None) moveDirection = dir;

	return moveDirection;
}

// 上に移動
void Guard::MoveUp()
{
	// 上に移動できない
	if (!IsMovableUp()) return;

	// 列に合わせるようにX座標を調整する
	AjustCloumn();

	// 上に移動する
	m_adjustPosition.y--;
	if (m_adjustPosition.y < 0)
	{
		m_adjustPosition.y = 4;
		// Page1を元に戻す
		m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
		// 上にタイルを移動
		m_tilePosition.y--;
		// プレイヤーなら死亡
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}
		// Page1に書き込む
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Guard);
	}

	// 金塊が拾えるかチェック
	CheckGoldPickedUp();

	//ステートを更新して金塊を落とす処理
	UpdateGoldDropTimer();

	// ハシゴを登るアニメーション
	SetGuardAnimationState(GuardAnimationState::Climb01, GuardAnimationState::Climb02);
}

// 下に移動
void Guard::MoveDown()
{
	// 下に移動できない
	if (!IsMovableDown()) return;

	// 列に合わせるようにX座標を調整する
	AjustCloumn();

	// 下に移動する
	m_adjustPosition.y++;
	if (m_adjustPosition.y > 4)
	{
		m_adjustPosition.y = 0;
		// Page1を元に戻す
		m_pLevel->CopyPage2toPage1(m_tilePosition.x, m_tilePosition.y);
		// 下にタイルを移動
		m_tilePosition.y++;
		// プレイヤーなら死亡
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}
		// Page1に書き込む
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Guard);
	}

	// 金塊が拾えるかチェック
	CheckGoldPickedUp();

	//ステートを更新して金塊を落とす処理
	UpdateGoldDropTimer();

	// ハシゴを登るアニメーション
	SetGuardAnimationState(GuardAnimationState::Climb01, GuardAnimationState::Climb02);
}

// 左に移動
void Guard::MoveLeft()
{
	// 左に移動できない
	if (!IsMovableLeft()) return;

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
		// 左にタイルを移動
		m_tilePosition.x--;
		// プレイヤーなら死亡
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}
		// Page1に書き込む
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Guard);
	}

	// 金塊が拾えるかチェック
	CheckGoldPickedUp();

	//ステートを更新して金塊を落とす処理
	UpdateGoldDropTimer();

	// ロープの場合
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Rope)
	{
		// ロープで移動するアニメーション
		SetGuardAnimationState(GuardAnimationState::Rope01_L, GuardAnimationState::Rope03_L);
	}
	else
	{
		// 走るアニメーション
		SetGuardAnimationState(GuardAnimationState::Run01_L, GuardAnimationState::Run03_L);
	}
}

// 右に移動
void Guard::MoveRight()
{
	// 右に移動できない
	if (!IsMovableRight()) return;

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
		// 右にタイルを移動
		m_tilePosition.x++;
		// プレイヤーなら死亡
		if (m_pLevel->GetTilePage1(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Player)
		{
			m_pGamePlayScene->GetPlayer()->SetAlive(false);
		}
		// Page1に書き込む
		m_pLevel->SetTilePage1(m_tilePosition.x, m_tilePosition.y, Tile::Type::Guard);
	}

	// 金塊が拾えるかチェック
	CheckGoldPickedUp();

	//ステートを更新して金塊を落とす処理
	UpdateGoldDropTimer();

	// ロープの場合
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Rope)
	{
		// ロープで移動するアニメーション
		SetGuardAnimationState(GuardAnimationState::Rope01_R, GuardAnimationState::Rope03_R);
	}
	else
	{
		// 走るアニメーション
		SetGuardAnimationState(GuardAnimationState::Run01_R, GuardAnimationState::Run03_R);
	}
}

// アニメーションステートの設定
void Guard::SetGuardAnimationState(GuardAnimationState beginAnimState, GuardAnimationState endAnimState)
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

	m_guardAnimationState = static_cast<GuardAnimationState>(animState);
}

// 金塊が拾えるか調べる関数
void Guard::CheckGoldPickedUp()
{
	// タイルの中心でなければチェックしない
	if (m_adjustPosition.x != Tile::TILE_CENTER_X) return;
	if (m_adjustPosition.y != Tile::TILE_CENTER_Y) return;

	// 金塊がない
	if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) != Tile::Type::Gold) return;

	// 既に金塊を持っている
	if (m_actionStateTimer < 0) return;

	// 金塊があれば金塊を拾う（金塊保持タイマーを設定）
	m_actionStateTimer = -0x53;

	// 空白にする
	m_pLevel->SetTilePage2(m_tilePosition.x, m_tilePosition.y, Tile::Type::Empty);
}

// 指定した行まで移動可能か調べる関数
bool Guard::IsMovableColumn(int column)
{
	int guardColumn = m_tilePosition.x;
	int guardRow = m_tilePosition.y;
	int move = 0;

	// 調べていく方向を設定
	if (guardColumn > column)
	{
		// 左へ
		move = -1;
	}
	else
	{
		// 右へ
		move = 1;
	}
	while (guardColumn != column)
	{
		// 次の行へ
		guardColumn += move;
		// ハシゴまたはロープなら下が何であっても移動可能
		if ( (m_pLevel->GetTilePage2(guardColumn, guardRow) == Tile::Type::Ladder)
		  || (m_pLevel->GetTilePage2(guardColumn, guardRow) == Tile::Type::Rope)
		   )
		{
			continue;
		}
		// 床が空白か罠でないなら移動可能
		if ( (guardRow == Level::MAX_GAME_ROW)
		  || ( (m_pLevel->GetTilePage2(guardColumn, guardRow + 1) != Tile::Type::Empty)
		    && (m_pLevel->GetTilePage2(guardColumn, guardRow + 1) != Tile::Type::Trap)
		     )
		   )
		{
			continue;
		}
		// 移動不可
		return false;
	}
	// 移動可能
	return true;
}

// 左右どこまで移動できるか取得する関数
void Guard::GetLeftRightLimits(int* colmun, Direction direction, int limit)
{
	*colmun = m_tilePosition.x;
	int row = m_tilePosition.y;

	int move = 1;
	if (direction == Direction::Left) move = -1;

	while (*colmun != limit)
	{
		// レンガまたは石なら終了
		if ( (m_pLevel->GetTilePage1(*colmun + move, row) == Tile::Type::Blick)		// レンガ
		  || (m_pLevel->GetTilePage1(*colmun + move, row) == Tile::Type::Stone)		// 石
		   )
		{
			break;
		}

		// ハシゴまたはロープでない
		if ( (m_pLevel->GetTilePage1(*colmun + move, row) != Tile::Type::Ladder)		// ハシゴ
		  && (m_pLevel->GetTilePage1(*colmun + move, row) != Tile::Type::Rope)			// ロープ
		   )
		{
			// 行が一番下でない
			if (row != Level::MAX_GAME_ROW)
			{
				// 床がレンガ、石、ハシゴでないなら終了
				if ( (m_pLevel->GetTilePage2(*colmun + move, row + 1) != Tile::Type::Blick)	// レンガ
				  && (m_pLevel->GetTilePage2(*colmun + move, row + 1) != Tile::Type::Stone)	// 石
				  && (m_pLevel->GetTilePage2(*colmun + move, row + 1) != Tile::Type::Ladder)	// ハシゴ
				   )
				{
					// 一応１つ先までは移動可能
					*colmun += move;
					break;
				}
			}
		}
		// 次へ
		*colmun += move;
	}
}

// 下方向に移動な最適な行を見つける関数
int Guard::FindCandidateRowBelow(int colmun, int row)
{
	// プレイヤーのいる行
	int playerRow = m_pGamePlayScene->GetPlayer()->GetTilePosition().y;

	// 一番下の行まで調べる
	while (row != Level::MAX_GAME_ROW)
	{
		// １つ下の行がレンガか石なら終了
		if ( (m_pLevel->GetTilePage2(colmun, row + 1) == Tile::Type::Blick)	// レンガ
		  || (m_pLevel->GetTilePage2(colmun, row + 1) == Tile::Type::Stone)	// 石
			)
		{
			return row;
		}
		// １行下へ
		row++;
		// 空白でない
		if (m_pLevel->GetTilePage2(colmun, row) != Tile::Type::Empty)
		{
			// 左側は移動可能なら
			if (CheckLeftRightMove(colmun, row, Direction::Left))
			{
				// プレイヤーの方が下の行なら
				if (row >= playerRow) return row;
			}
			// 右側は移動可能なら
			if (CheckLeftRightMove(colmun, row, Direction::Right))
			{
				// プレイヤーの方が下の行なら
				if (row >= playerRow) return row;
			}
		}
	}
	return row;
}

// 上方向に移動な最適な行を見つける関数
int Guard::FindCandidateRowAbove(int colmun, int row)
{
	// プレイヤーのいる行
	int playerRow = m_pGamePlayScene->GetPlayer()->GetTilePosition().y;

	// 一番上の行まで調べる
	while (row != 0)
	{
		// ハシゴでないので上に移動できないので終了
		if (m_pLevel->GetTilePage2(colmun, row) != Tile::Type::Ladder)
		{
			return row;
		}
		// １つ上の行をチェック
		row--;
		// 左側は移動可能なら
		if (CheckLeftRightMove(colmun, row, Direction::Left))
		{
			// プレイヤーの方が上の行なら
			if (row <= playerRow)
			{
				return row;
			}
		}
		// 右側は移動可能なら
		if (CheckLeftRightMove(colmun, row, Direction::Right))
		{
			// プレイヤーの方が上の行なら
			if (row <= playerRow)
			{
				return row;
			}
		}
	}
	return row;
}

// 左右に移動できそうか調べる関数
bool Guard::CheckLeftRightMove(int colmun, int row, Direction direction)
{
	// 指定方向の列を調べる
	if (direction == Direction::Left)
	{
		colmun--;
	}
	else if (direction == Direction::Right)
	{
		colmun++;
	}

	// 画面外ならチェックしない
	if ((colmun < 0) || (colmun > Level::MAX_GAME_COLMUN)) return false;

	if (m_pLevel->GetTilePage2(colmun, row) == Tile::Type::Rope)	// ロープ
	{
		return true;
	}
	else
	{
		// 左下がレンガ、石、ハシゴなら
		if ( (m_pLevel->GetTilePage2(colmun, row + 1) == Tile::Type::Blick)	// レンガ
		  || (m_pLevel->GetTilePage2(colmun, row + 1) == Tile::Type::Stone)	// 石
		  || (m_pLevel->GetTilePage2(colmun, row + 1) == Tile::Type::Ladder)	// ハシゴ
		   )
		{
			return true;
		}
	}
	return false;
}

// 上下へ移動するか調べる関数
//
// ガードの現在いる位置の上下を調べ最も疑似距離の近い方向へ移動する
Guard::MoveDirection Guard::SelectMoveUpAndDown(int* bestGuardDistance)
{
	MoveDirection moveDirection = MoveDirection::None;

	// ガードの位置
	int colmun = m_tilePosition.x;
	int row = m_tilePosition.y;

	// 下に移動可能なら
	if (IsMovableDown(colmun, row))
	{
		// 下に移動した時の最適な行を見つけてプレイヤーとの疑似距離を算出する
		int distance = GetPsuedoDistance(FindCandidateRowBelow(colmun, row));
		// 最も近い場合は
		if (distance < *bestGuardDistance)
		{
			// プレイヤーとの疑似距離を更新
			*bestGuardDistance = distance;
			// 下へ移動
			moveDirection = MoveDirection::Down;
		}
	}
	// 上へ移動可能なら
	if (IsMovableUp(colmun, row))
	{
		// 上に移動した時の最適な行を見つけてプレイヤーとの疑似距離を算出する
		int distance = GetPsuedoDistance(FindCandidateRowAbove(colmun, row));
		// 最も近い場合は
		if (distance < *bestGuardDistance)
		{
			// プレイヤーとの疑似距離を更新
			*bestGuardDistance = distance;
			// 上へ移動
			moveDirection = MoveDirection::UP;
		}
	}

	return moveDirection;
}

// 左へ移動するか調べる関数
//
// 移動可能な各行の上下方向を最もプレイヤーとの疑似距離が近い方向へ移動する
Guard::MoveDirection Guard::SelectMoveLeftAndRight(int* bestGuardDistance, int colmunLimit, Direction direction)
{
	MoveDirection moveDirection = MoveDirection::None;

	// ガードの位置
	int colmun = m_tilePosition.x;
	int row = m_tilePosition.y;

	// 現在のガードの位置まで調べたので左へ移動不可
	while (colmun != colmunLimit)
	{
		// 下に移動可能なら
		if (IsMovableDown(colmunLimit, row))
		{
			// 下に移動した時の最適な行を見つけてプレイヤーとの疑似距離を算出する
			int distance = GetPsuedoDistance(FindCandidateRowBelow(colmunLimit, row));
			// 最も近い場合は
			if (distance < *bestGuardDistance)
			{
				// プレイヤーとの疑似距離を更新
				*bestGuardDistance = distance;
				if (direction == Direction::Left)
				{
					// 左へ移動
					moveDirection = MoveDirection::Left;
				}
				else
				{
					// 右へ移動
					moveDirection = MoveDirection::Right;
				}
			}
		}

		// 上へ移動可能なら
		if (IsMovableUp(colmunLimit,row))
		{
			// 上に移動した時の最適な行を見つけてプレイヤーとの疑似距離を算出する
			int distance = GetPsuedoDistance(FindCandidateRowAbove(colmunLimit, row));
			// 最も近い場合は
			if (distance < *bestGuardDistance)
			{
				// プレイヤーとの疑似距離を更新
				*bestGuardDistance = distance;
				if (direction == Direction::Left)
				{
					// 左へ移動
					moveDirection = MoveDirection::Left;
				}
				else
				{
					// 右へ移動
					moveDirection = MoveDirection::Right;
				}
			}
		}

		if (direction == Direction::Left)
		{
			// １つ右の列へ調べる位置を移動
			colmunLimit++;
		}
		else
		{
			// １つ左の列へ調べる位置を移動
			colmunLimit--;
		}
	}

	return moveDirection;
}

// 下に移動可能なタイルか調べる関数
bool Guard::IsMovableDown(int colmun, int row)
{
	if ( (row != Level::MAX_GAME_ROW)	// １番下の行ではなく
	  && (m_pLevel->GetTilePage2(colmun, row + 1) != Tile::Type::Blick)	// レンガでない
	  && (m_pLevel->GetTilePage2(colmun, row + 1) != Tile::Type::Stone)	// 石でない
	   )
	{
		return true;
	}
	return false;
}

// 上に移動可能なタイルか調べる関数
bool Guard::IsMovableUp(int colmun, int row)
{
	if ( (row != 0)	// 一番上の行でなく
	  && (m_pLevel->GetTilePage2(colmun, row) == Tile::Type::Ladder)	// ハシゴがある
	   )
	{
		return true;
	}
	return false;
}

// 移動可能なタイルか調べる関数（落下）
bool Guard::IsMovableTileFall(Tile::Type page1, Tile::Type page2)
{
	// 空白、プレイヤーなら落下
	if ((page1 == Tile::Type::Empty) || (page1 == Tile::Type::Player))
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

//ステートを更新して金塊を落とす処理
void Guard::UpdateGoldDropTimer()
{
	// 行動タイマーが０になったら落とす
	if (m_actionStateTimer < 0)
	{
		m_actionStateTimer++;
		if (m_actionStateTimer == 0)
		{
			// 空白なら金塊を落とす
			if (m_pLevel->GetTilePage2(m_tilePosition.x, m_tilePosition.y) == Tile::Type::Empty)
			{
				m_pLevel->SetTilePage2(m_tilePosition.x, m_tilePosition.y, Tile::Type::Gold);
			}
			else
			{
				// 次の機会を待つ
				m_actionStateTimer--;
			}
		}
	}
}

// プレイヤーとの疑似距離を求める関数
//
// 距離が近いほど選択されやすくなる
// 選択されやすい順序
//   １・プレイヤーが同じ行
//   ２・プレイヤーが下 (+100)
//   ３・プレイヤーが上 (+200)
int Guard::GetPsuedoDistance(int row)
{
	// プレイヤーの位置
	int playerColmun = m_pGamePlayScene->GetPlayer()->GetTilePosition().x;
	int playerRow = m_pGamePlayScene->GetPlayer()->GetTilePosition().y;

	// 同じ行にいる場合
	if (row == playerRow)
	{
		// プレイヤーとの距離を返す
		return abs(playerColmun - m_tilePosition.x);
	}
	// プレイヤーが上にいる
	else if(row > playerRow)
	{
		return 200 + (row - playerRow);
	}
	// プレイヤーが下にいる
	return 100 + (playerRow - row);
}

// 移動可能なタイルか調べる関数（上左右）
bool Guard::IsMovableTileULR(Tile::Type tile)
{
	if ((tile == Tile::Type::Blick)	// page1:レンガ
		|| (tile == Tile::Type::Stone)	// page1:石
		|| (tile == Tile::Type::Guard)	// page1:ガード
		|| (tile == Tile::Type::Trap)	// page1:罠（罠は上から以外は移動不可）
		)
	{
		// 移動不可
		return false;
	}
	// 移動可能
	return true;
}

// 移動可能なタイルか調べる関数（下）
bool Guard::IsMovableTileD(Tile::Type tile)
{
	if ((tile == Tile::Type::Blick)	// page1:レンガ
		|| (tile == Tile::Type::Stone)	// page1:石
		|| (tile == Tile::Type::Guard)	// page1:ガード
		)
	{
		// 移動不可
		return false;
	}
	// 移動可能
	return true;
}


