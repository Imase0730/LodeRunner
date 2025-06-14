﻿/**
 * @file   Game.h
 *
 * @brief  ゲーム全体に関するヘッダファイル
 *
 * @author 制作者名
 *
 * @date   日付
 */

// 多重インクルードの防止 =====================================================
#pragma once




// ヘッダファイルの読み込み ===================================================

#include "TitleScene/TitleScene.h"
#include "GamePlayScene/GamePlayScene.h"
#include "LevelEditScene/LevelEditScene.h"

#include "Game/IrisWipe.h"

// クラスの宣言 ===============================================================




// クラスの定義 ===============================================================
/**
 * @brief ゲーム
 */
class Game
{
// 列挙型の定義 -----------------------------------------------------
public:

	// シーンID
	enum class SceneID
	{
		None = -1,
		Title,		// タイトル
		GamePlay,	// ゲームプレイ
		LevelEdit,	// レベルエディット
	};

// クラス定数の宣言 -------------------------------------------------
public:
	// システム関連
	static constexpr const wchar_t* TITLE = L"Lode Runner";   ///< ゲームタイトル

// データメンバの宣言 -----------------------------------------------
private:
	// 入力関連
	int m_key;       ///< 現在のキー情報
	int m_oldKey;    ///< 前回のキー情報

	SceneID m_currentSceneID;	// 現在のシーンID
	SceneID m_requestedSceneID;	// 変更要求のシーンID

	// シーンオブジェクト
	TitleScene m_titleScene;			// タイトルシーン
	GamePlayScene m_gamePlayScene;		// ゲームプレイシーン
	LevelEditScene m_levelEditScene;	// レベルエディットシーン

	// 描画先のグラフィックハンドル
	int m_ghScreen;

	// タイルセットのグラフィックハンドル
	int m_ghTileset;

	// アイリスワイプ
	IrisWipe m_irisWipe;

	// メンバ関数の宣言 -------------------------------------------------
// コンストラクタ/デストラクタ
public:
	// コンストラクタ
	Game();

	// デストラクタ
	~Game();


// 操作
public:
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render();

	// 終了処理
	void Finalize();

	// シーン変更の要求
	void RequestSceneChange(SceneID nextSceneID);

	// アイリスワイプを取得する関数
	IrisWipe* GetIrisWipe() { return &m_irisWipe; }

private:

	// 開始シーンの設定
	void SetStartScene(SceneID startSceneID);

	// シーンの変更
	void ChangeScene();

	// 現在のシーンの初期化処理
	void InitializeCurrentScene();

	// 現在のシーンの更新処理
	void UpdateCurrentScene(int keyCondition, int keyTrigger);

	// 現在のシーンの描画処理
	void RenderCurrentScene();

	// 現在のシーンの終了処理
	void FinalizeCurrentScene();

};
