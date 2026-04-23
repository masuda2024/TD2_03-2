#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

class Enemy;
class E_Bullet;
class P_Bullet
{
public:
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);
	void Update();
	void Draw(const KamataEngine::Camera& camera);

	KamataEngine::Model* model_P_Bullet_ = nullptr;

	// 速度
	KamataEngine::Vector3 PB_velocity_;

#pragma region 衝突判定 [ プレイヤーの弾  <<===>>  敵 ]

	// 当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();
	// 衝突応答
	void OnCollition(const Enemy* enemy);

#pragma endregion



#pragma region 衝突判定 [ プレイヤーの弾  <<===>>  敵の弾 ]

	// AABBを取得
	AABB4 GetAABB4();
	// 衝突応答
	void OnCollition4(const E_Bullet* e_bullet);

#pragma endregion


	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// 寿命
	static const int32_t kLifeTime = 30 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_pb_ = false;
	bool IsDead_PB() const { return isDead_pb_; }

private:
	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_;

	KamataEngine::Vector3 velocity_ = {};

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	
};
