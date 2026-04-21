#include "P_Bullet.h"
#include "KamataEngine.h"
using namespace KamataEngine;
using namespace MathUtility;
#include "MyMath.h"

void P_Bullet::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity)
{
	assert(model);

	model_ = model;

	model_P_Bullet_ = model;

	PB_velocity_ = velocity;

	model_P_Bullet_ = KamataEngine::Model::CreateFromOBJ("Z_bullet");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}
void P_Bullet::Update()
{
	deathTimer_--;
	if (--deathTimer_ <= 0)
	{
		isDead_pb_ = true;
	}
	


	// 座標を移動させる (1フレーム分の移動量を足し込む)
	worldTransform_.translation_.x += PB_velocity_.x;
	worldTransform_.translation_.y += PB_velocity_.y;

	// アフィン変換行列
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}
void P_Bullet::Draw(const KamataEngine::Camera& camera) { model_P_Bullet_->Draw(worldTransform_, camera); }

#pragma region 衝突判定 [ プレイヤーの弾  <<===>>  敵 ]

KamataEngine::Vector3 P_Bullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB P_Bullet::GetAABB() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 弾と敵の衝突応答
void P_Bullet::OnCollition(const Enemy* enemy) 
{
	(void)enemy;
	isDead_pb_ = true;
}

#pragma endregion

#pragma region 衝突判定 [ プレイヤーの弾  <<===>>  敵の弾 ]

AABB4 P_Bullet::GetAABB4()
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB4 aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 弾と敵の衝突応答
void P_Bullet::OnCollition4(const E_Bullet* e_bullet)
{ 
	(void)e_bullet; 


	isDead_pb_ = true;



}

#pragma endregion




