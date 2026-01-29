#pragma once
#include "KamataEngine.h"


#include "MapChipField.h"


#include "CameraController.h"
#include "Skydome.h"


#include "Player.h"
#include "P_DeathParticle.h"
#include "P_Bullet.h"


#include "Enemy.h"
#include "E_DeathParticle.h"



#include "Fade.h"
#include <list>
#include <vector>
#define NOMINMAX
#include <Windows.h>
#include <algorithm>
// ゲームシーン

using namespace KamataEngine;

class Game
{
public:
	// デストラクタ
	~Game();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 終了フラグ
	bool finishedGAME_ = false;
	// デス(オーバー)フラグのgetter
	bool IsFinishedGAME() const { return finishedGAME_; } // ゲームオーバー

	// 敵を倒した場合の(クリア)フラグ
	bool finishedGAME2_ = false;
	bool IsFinishedGAME2() const { return finishedGAME2_; } ////ゲームクリア

private:
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	
	// デバックカメラの生成
	// debugCamera_ = new DebugCamera();

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera camera_;

	// スプライト
	KamataEngine::Sprite* sprite_ = nullptr;




	//ブロック
	KamataEngine::Model* modelBlock_;
	// マップチップフィールド
	MapChipField* mapChipField_;
	
	void GenerateBlocks();



	// 得点テクスチャハンドル
	uint32_t pointHandle_ = 0;
	// スプライト
	KamataEngine::Sprite* pointSprite_ = nullptr;





	
	// スコア
	int score = 0;


	



	// 天球
	Skydome* skydome_ = nullptr;
	KamataEngine::Model* modelskydome_ = nullptr;

#pragma region プレイヤー

	// プレイヤー
	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

#pragma region プレイヤーの弾
	// 自キャラの弾
	KamataEngine::Model* modelPlayerBullet_ = nullptr;
	// 弾
	std::list<P_Bullet*> bullets_;
	// 速度
	KamataEngine::Vector3 velocity_;
	// 弾の寿命(フレーム数)
	//int playerBulletLifeTime = 20; // 変更可能な左辺値にするためconstを外し型をintに変更    // スペースキーを押して弾を撃つ

#pragma endregion

	// プレイヤーの弾の生成
	P_Bullet* playerBullet_ = nullptr;
	

	// プレイヤーのパーティクル
	P_DeathParticle* P_Particles_ = nullptr;
	KamataEngine::Model* model_P_Particle_ = nullptr;

	

	

#pragma endregion

#pragma region 敵

	// 敵
	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;


	KamataEngine::Model* modelEnemy_ = nullptr;

	// 敵のパーティクル
	E_DeathParticle* E_Particles_ = nullptr;
	KamataEngine::Model* model_E_Particle_ = nullptr;

	int enemyPos = 0;

	int respawnTimer = 120;

#pragma endregion

	

#pragma region カメラ関係

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;
	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	// カメラコントロール
	CameraController* cameraController_ = nullptr;
	
#pragma endregion




	// 全ての当たり判定
	void CheckAllCollisions();

#pragma region フェーズ・フェード

	// ゲームのフェーズ(型)
	enum class Phase
	{
		kFadeIn,       // フェードイン
		kPlay,         // ゲームプレイ
		kDeath,        // プレイヤーのデス演出
		kBarrierDeath, // バリアのデス演出
		kEnemyDeath,   // 敵のデス演出
		kFadeOut,      // フェードアウト(オーバー)
		kFadeOut2,     // フェードアウト(クリア)
	};

	// ゲームの現在フェーズから開始
	Phase phase_;

	// フェーズの切り替え
	void ChangePhase();

	// フェード
	Fade* fade_ = nullptr;
#pragma endregion

	

};
