//--------------------------------------------------------------------------------------
// File: IrisWipe.cpp
//
//--------------------------------------------------------------------------------------
#include "IrisWipe.h"

// マスクデータ
unsigned char IrisWipe::m_maskData[Level::LEVEL_SCREEN_HEIGHT][Level::LEVEL_SCREEN_WIDTH]{};

// コンストラクタ
IrisWipe::IrisWipe()
	: m_counter{ 0 }
	, m_isUse{ false }
	, m_isActive{ false }
	, m_mode{ Mode::None }
{
}

// デストラクタ
IrisWipe::~IrisWipe()
{
	// 終了処理
	Finalize();
}

// 初期化処理
void IrisWipe::Initialize(Mode mode)
{
	// ワイプ実行中は初期化できない
	if (m_isActive) return;

	// モードを設定
	m_mode = mode;

	// カウンターの初期化
	m_counter = 0;

	// マスク画面を作成します
	if (m_isUse == false)
	{
		CreateMaskScreen();
		m_isUse = true;
	}

	// マスクデータを初期化
	for (int i = 0; i < Level::Level::LEVEL_SCREEN_HEIGHT; i++)
	{
		for (int j = 0; j < Level::LEVEL_SCREEN_WIDTH; j++)
		{
			m_maskData[i][j] = 0xff;
		}
	}
}

// 更新処理
void IrisWipe::Update()
{
	if (m_mode == Mode::None) return;

	// 黒で初期化
	for (int i = 0; i < Level::Level::LEVEL_SCREEN_HEIGHT; i++)
	{
		for (int j = 0; j < Level::LEVEL_SCREEN_WIDTH; j++)
		{
			m_maskData[i][j] = 0xff;
		}
	}

	// アイリスワイプ用の円を作成する
	int radius = UpdateCounter();
	int cx = WIPE_CENTER_X;
	int cy = WIPE_CENTER_Y;
	int r2 = radius * radius;
	for (int y = -radius; y <= radius; y++)
	{
		for (int x = -radius; x <= radius; x++)
		{
			// 円を少しガタガタにみせる
			int xx = (x / WIPE_PIXEL_WIDTH) * WIPE_PIXEL_WIDTH;
			int yy = (y / WIPE_PIXEL_HEIGHT) * WIPE_PIXEL_HEIGHT;
			if ((xx * xx + yy * yy) < r2)
			{
				xx = cx + x;
				yy = cy + y;
				// 配列外じゃないかチェック
				if ( (xx >= 0) && (xx < Level::LEVEL_SCREEN_WIDTH)
				  && (yy >= 0) && (yy < Level::LEVEL_SCREEN_HEIGHT)
				   )
				{
					m_maskData[yy][xx] = 0;
				}
			}
		}
	}

}

// 描画処理
void IrisWipe::Render() const
{
	if (m_mode == Mode::None) return;

	// マスクのデータをマスク画面に直接描画する
	DrawMaskToDirectData(0, 0, Level::LEVEL_SCREEN_WIDTH, Level::LEVEL_SCREEN_HEIGHT, m_maskData, DX_MASKTRANS_BLACK);
}

// 終了処理
void IrisWipe::Finalize()
{
	// マスク画面を削除します
	if (m_isUse)
	{
		DeleteMaskScreen();
		m_isUse = false;
	}
}

// ワイプを開始する関数
void IrisWipe::Start(Mode mode)
{
	Initialize(mode);
	m_isActive = true;
}

// カウンターの更新関数
int IrisWipe::UpdateCounter()
{
	// マスク動作中のみ更新を行う
	if (!m_isActive) return 0;

	// ワイプ処理終了？
	if (m_counter == WIPE_COUNTER)
	{
		// マスク動作中フラグ(OFF)
		m_isActive = false;
		// オープンの時
		if (m_mode == Mode::Open)
		{
			// 画面からマスクは削除する
			m_mode = Mode::None;
			Finalize();
		}
	}
	else
	{
		m_counter++;
	}
	// クローズの時はカウンター増減を逆にする
	if (m_mode == Mode::Close)
	{
		return WIPE_COUNTER - m_counter;
	}
	return m_counter;
}

