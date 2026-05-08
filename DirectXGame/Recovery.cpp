#include "Recovery.h"
using namespace KamataEngine;
using namespace MathUtility;


void Recovery::Initialize(Model* model, Camera* camera, Vector3& position)
{
	// NULLポインタチェック
	assert(model);

	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	//textureHandle_ = textureHandle;
}

void Recovery::Update() 
{ 
	/*
	if (Input::GetInstance()->TriggerKey(DIK_R))
	{
		isDelete_recovery_ = true;
	}*/

	// アフィン変換行列
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Recovery::Draw() 
{
	if (isDelete_recovery_)
	{
		return;
	}
	model_->Draw(worldTransform_, *camera_);
}


KamataEngine::Vector3 Recovery::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

#pragma region プレイヤーと回復アイテム

AABB3 Recovery::GetAABB3() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB3 aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 衝突応答
void Recovery::OnCollition3(const Player* player)
{
	(void)player;
	

	//isDelete_recovery_ = true;



}

#pragma endregion
