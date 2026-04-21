#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

class Player;
class P_Bullet;
class E_Bullet
{
public:
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);
	void Update();
	void Draw(const KamataEngine::Camera& camera);

	KamataEngine::Model* model_E_Bullet_ = nullptr;

	// 速度
	KamataEngine::Vector3 EB_velocity_;

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

#pragma region 衝突判定 [ プレイヤー  <<===>>  敵の弾 ]

	// 当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	// AABBを取得
	AABB2 GetAABB2();
	// 衝突応答
	void OnCollition2(const Player* player);

#pragma endregion


#pragma region 衝突判定 [ プレイヤーの弾  <<===>>  敵の弾 ]



	// AABBを取得
	AABB4 GetAABB4();
	// 衝突応答
	void OnCollition4(const P_Bullet* p_bullet);



#pragma endregion








	// 寿命
	static const int32_t kLifeTime_E = 60 * 5;
	// デスタイマー
	int32_t deathTimer_E_ = kLifeTime_E;
	// デスフラグ
	bool isDead_eb_ = false;
	bool IsDead_EB() const { return isDead_eb_; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_;

	KamataEngine::Vector3 velocity_ = {};

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
};
