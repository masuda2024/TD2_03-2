#include "E_Bullet.h"
#include "Enemy.h"

#include <list>
#define NOMINMAX
#include "MapChipField.h"

using namespace KamataEngine;
using namespace MathUtility;

void E_Bullet::Initialize(KamataEngine::Model* model, Camera* camera, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity)
{
	// NULLポイントチェック
	// assert(model);

	model_ = model;

	camera_ = camera;

	// 速度を引数で受け取って初期化
	velocity_ = velocity;

	// 引数で受け取った初期座標をリセット
	worldTransform_.translation_ = position;

	// ワールド変換データ初期化
	worldTransform_.Initialize();

	// NextActionTime_ = (rand() % 300) / 2.0f + 60.0f;
	//  Bulletvelocity_ = velocity;
	isShot = true;

	// shotT = 30.0f;
	// shotC = 40.0f;
}

void E_Bullet::Update()
{
	/*
	if (isShot == 0)
	{
	    shotC--;
	    if (shotC <= 0.0f)
	    {
	        isShot = true;
	        shotT = 30.0f;
	    }
	}

	if (isShot == 1)
	{

	    shotT--;
	    if (shotT <= 0.0f)
	    {
	        isShot = false;
	        shotC = 40.0f;
	    }
	}*/

	worldTransform_.translation_ -= velocity_;

	// アフィン変換行列
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix(); // 敵の座標の計算
}

void E_Bullet::Draw()
{

	// モデルの描画

	if (isShot == 1)
	{
		model_->Draw(worldTransform_, *camera_);
	}

	// 終了なら何もしない
	if (isFinished_)
	{
		return;
	}
}

KamataEngine::Vector3 E_Bullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

#pragma region 敵の弾とプレイヤーの当たり判定

AABB2 E_Bullet::GetAABB2() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB2 aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// プレイヤーと敵の弾の衝突
void E_Bullet::OnCollition2(const Player* player) { (void)player; }

#pragma endregion