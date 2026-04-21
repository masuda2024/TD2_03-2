#pragma once
#include "KamataEngine.h"

#include "MyMath.h"


#include "CameraController.h"




#include "Fade.h"


#include "Player.h"
#include "Enemy.h"

#include "P_Bullet.h"
#include "E_Bullet.h"

#include "Recovery.h"
class Game
{
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// デストラクタ
	~Game();

	// 終了フラグ
	bool finishedGAME_ = false;
	// デス(オーバー)フラグのgetter
	bool IsFinishedGAME() const { return finishedGAME_; } // ゲームオーバー

	// 敵を倒した場合の(クリア)フラグ
	bool finishedGAME2_ = false;
	bool IsFinishedGAME2() const { return finishedGAME2_; } ////ゲームクリア

	// 衝突判定
	void CheckAllCollisions();

private:
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;
	// カメラ
	KamataEngine::Camera camera_;

#pragma region UI

	// プレイヤーのHP
	uint32_t playerHPHandle_ = 0;
	Sprite* playerHPSprite_ = nullptr;

	uint32_t _playerHPHandle_ = 0;
	Sprite* _playerHPSprite_ = nullptr;

	// 敵のHP
	//  敵HPテクスチャハンドル
	uint32_t enemyHPHandle_ = 0;
	// スプライト
	Sprite* enemyHPSprite_ = nullptr;

	uint32_t _enemyHPHandle_ = 0;
	Sprite* _enemyHPSprite_ = nullptr;

#pragma endregion

#pragma region 天球

	KamataEngine::Model* modelSkydome_ = nullptr;
	KamataEngine::Model* modelEarth_ = nullptr;
	KamataEngine::Model* modelMoon_ = nullptr;

	KamataEngine::WorldTransform worldTransformEarth_;
	KamataEngine::WorldTransform worldTransformMoon_;
#pragma endregion

#pragma region プレイヤー
	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

#pragma endregion

#pragma region 敵
	Enemy* enemy_ = nullptr;
	KamataEngine::Model* modelEnemy_ = nullptr;
#pragma endregion

#pragma region 回復アイテム

	//Recovery* recovery_ = nullptr;
	
	KamataEngine::Model* modelRecovery_ = nullptr;

	

#pragma endregion

	// プレイヤーの弾の生成
	P_Bullet* playerBullet_ = nullptr;

	// 敵の弾の生成
	E_Bullet* E_Bullet_ = nullptr;

#pragma region フェーズ・フェード

	// ゲームのフェーズ(型)
	enum class Phase
	{
		kFadeIn,     // フェードイン
		kPlay,       // ゲームプレイ
		kDeath,      // プレイヤーのデス演出
		kEnemyDeath, // 敵のデス演出
		kFadeOut,    // フェードアウト(オーバー)
		kFadeOut2,   // フェードアウト(クリア)
	};

	// ゲームの現在フェーズから開始
	Phase phase_;

	// フェーズの切り替え
	// void ChangePhase();

	// フェード
	Fade* fade_ = nullptr;
#pragma endregion

#pragma region カメラ関係

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;
	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	// カメラコントロール
	CameraController* cameraController_ = nullptr;

#pragma endregion
};
