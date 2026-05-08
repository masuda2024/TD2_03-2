#pragma once

#include "KamataEngine.h"

#include <algorithm>
#include <cassert>
#include <list>
#include <numbers>
#define NOMINMAX
#include "MyMath.h"
#include "math.h"
#include <cmath>

#include "E_Bullet.h"

class P_Bullet;
class Enemy 
{
public:
	// 敵の弾
	std::list<E_Bullet*> e_bullets_;

#pragma region 基本構成
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	~Enemy();
#pragma endregion

#pragma region 敵の状態
	// 体力表示
	int32_t E_GetHP() const { return E_hp_; }
	int32_t E_GetMaxHP() const { return E_maxHP_; }
	bool IsEnemyDead() const { return isEnemyDead_; }
	// デスフラグ
	bool isEnemyDead_ = false;

	// 行動フェーズ
	enum class Phase
	{
		Approach,
		Attack,
		Rage, // HP低いとき
	};
	Phase phase_ = Phase::Approach;

#pragma endregion

#pragma region 衝突判定 [ プレイヤーの弾  <<===>>  敵 ]

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.5f;
	static inline const float kHeight = 2.5f;

	// void OnCollisionE();
	const std::list<E_Bullet*>& GetE_Bullets() const { return e_bullets_; }

	// AABBを取得
	AABB GetAABB();
	// 衝突応答
	void OnCollition(const P_Bullet* playerBullet);
#pragma endregion

#pragma region 敵の攻撃

	void Fire();
	void ApproachInitialize();
	// 発射間隔
	static const int kFireInterval = 30;

#pragma endregion

#pragma region 座標(ワールド変換)

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	const KamataEngine::Vector3& GetRotation() const { return worldTransform_.rotation_; }

#pragma endregion

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_;

	KamataEngine::Vector3 velocity_ = {};

	int32_t E_maxHP_ = 10000;
	int32_t E_hp_ = E_maxHP_;

	// 発射タイマー
	int32_t fireTimer_ = 0;


	float walkTimer_ = 0;
};
