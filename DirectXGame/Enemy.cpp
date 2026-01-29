#include "Enemy.h"
#include"Game.h"
#include"cassert"
#include<numbers>
#include<algorithm>
#include<list>
#define NOMINMAX


using namespace KamataEngine;
using namespace MathUtility;

void Enemy::Initialize(Model* model, Camera* camera, KamataEngine::Vector3& position) 
{
	// NULLポイントチェック
	assert(model);

	model_ = model;

	// textureHandle_ = textureHandle;

	worldTransform_.translation_ = position;

	camera_ = camera;

	// 速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
	

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;

	worldTransform_.Initialize();

	
	
	
}

void Enemy::Update() 
{

	// 時間のカウンター
	walkTimer_ += 5.0f / 60.0f; // フレームごとの時間増分

	

	if (point >= maxP_)
	{
		isenemyDead2_ = true;
	}

	// プレイヤーの座標の計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Enemy::Draw() 
{
	if (isenemyDead_)
	{
		
		respawnTimer--;
		if (respawnTimer <= 0)
		{
			isenemyDead_ = false;
			respawnTimer = 120;
		}

		return;
		
	}
	model_->Draw(worldTransform_, *camera_);
}

KamataEngine::Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

#pragma region プレイヤーの弾と敵

AABB Enemy::GetAABB()
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 衝突応答
void Enemy::OnCollition(const P_Bullet* playerBullet)
{
	(void)playerBullet;
	isenemyDead_=true;	
	
}

#pragma endregion