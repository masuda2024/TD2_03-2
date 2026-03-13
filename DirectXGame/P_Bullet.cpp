#include "P_Bullet.h"
#include "KamataEngine.h"

#include "MapChipField.h"

#include"Player.h"
#include"Game.h"


#include <algorithm>
#include <list>
#include <numbers>
#define NOMINMAX


using namespace KamataEngine;
using namespace MathUtility;

#pragma region 行列

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	result.m[0][0] = {(m1.m[0][0] * m2.m[0][0]) + (m1.m[0][1] * m2.m[1][0]) + (m1.m[0][2] * m2.m[2][0]) + (m1.m[0][3] * m2.m[3][0])};
	result.m[0][1] = {(m1.m[0][0] * m2.m[0][1]) + (m1.m[0][1] * m2.m[1][1]) + (m1.m[0][2] * m2.m[2][1]) + (m1.m[0][3] * m2.m[3][1])};
	result.m[0][2] = {(m1.m[0][0] * m2.m[0][2]) + (m1.m[0][1] * m2.m[1][2]) + (m1.m[0][2] * m2.m[2][2]) + (m1.m[0][3] * m2.m[3][2])};
	result.m[0][3] = {(m1.m[0][0] * m2.m[0][3]) + (m1.m[0][1] * m2.m[1][3]) + (m1.m[0][2] * m2.m[2][3]) + (m1.m[0][3] * m2.m[3][3])};

	result.m[1][0] = {(m1.m[1][0] * m2.m[0][0]) + (m1.m[1][1] * m2.m[1][0]) + (m1.m[1][2] * m2.m[2][0]) + (m1.m[1][3] * m2.m[3][0])};
	result.m[1][1] = {(m1.m[1][0] * m2.m[0][1]) + (m1.m[1][1] * m2.m[1][1]) + (m1.m[1][2] * m2.m[2][1]) + (m1.m[1][3] * m2.m[3][1])};
	result.m[1][2] = {(m1.m[1][0] * m2.m[0][2]) + (m1.m[1][1] * m2.m[1][2]) + (m1.m[1][2] * m2.m[2][2]) + (m1.m[1][3] * m2.m[3][2])};
	result.m[1][3] = {(m1.m[1][0] * m2.m[0][3]) + (m1.m[1][1] * m2.m[1][3]) + (m1.m[1][2] * m2.m[2][3]) + (m1.m[1][3] * m2.m[3][3])};

	result.m[2][0] = {(m1.m[2][0] * m2.m[0][0]) + (m1.m[2][1] * m2.m[1][0]) + (m1.m[2][2] * m2.m[2][0]) + (m1.m[2][3] * m2.m[3][0])};
	result.m[2][1] = {(m1.m[2][0] * m2.m[0][1]) + (m1.m[2][1] * m2.m[1][1]) + (m1.m[2][2] * m2.m[2][1]) + (m1.m[2][3] * m2.m[3][1])};
	result.m[2][2] = {(m1.m[2][0] * m2.m[0][2]) + (m1.m[2][1] * m2.m[1][2]) + (m1.m[2][2] * m2.m[2][2]) + (m1.m[2][3] * m2.m[3][2])};
	result.m[2][3] = {(m1.m[2][0] * m2.m[0][3]) + (m1.m[2][1] * m2.m[1][3]) + (m1.m[2][2] * m2.m[2][3]) + (m1.m[2][3] * m2.m[3][3])};

	result.m[3][0] = {(m1.m[3][0] * m2.m[0][0]) * (m1.m[3][1] * m2.m[1][0]) + (m1.m[3][2] * m2.m[2][0]) + (m1.m[3][3] * m2.m[3][0])};
	result.m[3][1] = {(m1.m[3][0] * m2.m[0][1]) + (m1.m[3][1] * m2.m[1][1]) + (m1.m[3][2] * m2.m[2][1]) + (m1.m[3][3] * m2.m[3][1])};
	result.m[3][2] = {(m1.m[3][0] * m2.m[0][2]) + (m1.m[3][1] * m2.m[1][2]) + (m1.m[3][2] * m2.m[2][2]) + (m1.m[3][3] * m2.m[3][2])};
	result.m[3][3] = {(m1.m[3][0] * m2.m[0][3]) + (m1.m[3][1] * m2.m[1][3]) + (m1.m[3][2] * m2.m[2][3]) + (m1.m[3][3] * m2.m[3][3])};

	return result;
}

Matrix4x4 MakeRotateMatrix(const Vector3& rotation)
{
	float cosX = cosf(rotation.x);
	float sinX = sinf(rotation.x);

	float cosY = cosf(rotation.y);
	float sinY = sinf(rotation.y);

	float cosZ = cosf(rotation.z);
	float sinZ = sinf(rotation.z);

	// X回転
	Matrix4x4 rotX = {1, 0, 0, 0, 0, cosX, sinX, 0, 0, -sinX, cosX, 0, 0, 0, 0, 1};

	// Y回転
	Matrix4x4 rotY = {cosY, 0, -sinY, 0, 0, 1, 0, 0, sinY, 0, cosY, 0, 0, 0, 0, 1};

	// Z回転
	Matrix4x4 rotZ = {cosZ, sinZ, 0, 0, -sinZ, cosZ, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

	// Z → X → Y の順で合成
	Matrix4x4 result = Multiply(Multiply(rotZ, rotX), rotY);

	return result;
}

#pragma endregion

void P_Bullet::Initialize(KamataEngine::Model* model, Camera* camera, Player* player)
{
	// NULLポイントチェック
	// assert(model);

	model_ = model;

	camera_ = camera;

	player_ = player;

	// ワールド変換データ初期化
	worldTransform_.Initialize();
}

void P_Bullet::Update() 
{
	/*
	if (Input::GetInstance()->TriggerKey(DIK_M))
	{
		ON_Mouse = !ON_Mouse;  // 押すたびに反転
		OFF_Mouse = !ON_Mouse; // 逆状態にする
	}

	
	if (Input::GetInstance()->TriggerKey(DIK_M)) 
	{
		useMouseAttack_ = !useMouseAttack_;
	}
	if (!isActive_ && Input::GetInstance()->IsTriggerMouse(0))
{
	if (useMouseAttack_)
	{
	    StartAttack_at_Mouse();
	}
	else
	{
	    StartAttack();
	}
}
	*/



	if (!isActive_)
	{
		return;
	}

	CollisionMapInfo collisionMapInfo{};
	collisionMapInfo.move = velocity_;

	CheckMapCollision(collisionMapInfo);
	CheckMapHit(collisionMapInfo);

	// 弾を移動
	worldTransform_.translation_ += velocity_;

	
	// 弾が表示されている時間
	timer_ += 1.0f / 60.0f;
	if (timer_ >= kLifeTime)
	{
		isActive_ = false;
		return;
	}

	// アフィン変換行列
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix(); // プレイヤーの座標の計算
}


/*
void P_Bullet::StartAttack()
{
	
	if (!player_)
		return;
	isActive_ = true;
	timer_ = 0.0f;
	

	const auto& rot = player_->GetRotation();

	float pitch = rot.x; // 上下
	float yaw = rot.y;   // 左右

	// Player の向き通りの forward

	KamataEngine::Vector3 forward;
	forward.x = cosf(pitch) * sinf(yaw);
	forward.y = -sinf(pitch);
	forward.z = -cosf(pitch) * cosf(yaw);

	velocity_ = forward;

	const float kSpawnOffset = 1.5f;
	worldTransform_.translation_ = player_->GetWorldPosition() + forward * kSpawnOffset;

}
*/


void P_Bullet::StartAttack_at_Mouse()
{
	if (!player_)
		return;
	isActive_ = true;
	timer_ = 0.0f;
	

	// マウス座標
	Vector2 mousePos = Input::GetInstance()->GetMousePosition();

	float screenWidth = 1280.0f;
	float screenHeight = 720.0f;

	// -1～1 に変換
	float ndcX = (mousePos.x / screenWidth) * 2.0f - 1.0f;
	float ndcY = 1.0f - (mousePos.y / screenHeight) * 2.0f;

	// カメラ空間の方向
	float tanFov = tanf(camera_->fovAngleY * 0.5f);

	Vector3 rayDirCamera;
	rayDirCamera.x = ndcX * camera_->aspectRatio * tanFov;
	rayDirCamera.y = ndcY * tanFov;
	rayDirCamera.z = 0.0f;

	rayDirCamera = Normalize(rayDirCamera);

	// カメラ回転取得
	Vector3 camRot = camera_->rotation_;

	Matrix4x4 rotMat = MakeRotateMatrix(camRot);

	// ワールド方向に変換
	Vector3 dir = TransformNormal(rayDirCamera, rotMat);
	dir = Normalize(dir);

	// 弾速度
	float bulletSpeed = 0.5f;
	velocity_ = dir * bulletSpeed;

	// 発射位置
	Vector3 playerPos = player_->GetWorldPosition();
	const float kSpawnOffset = 1.5f;
	worldTransform_.translation_ = playerPos + dir * kSpawnOffset;
}







#pragma region ブロックとの衝突

void P_Bullet::CheckMapCollision(CollisionMapInfo& info)
{
	CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

void P_Bullet::CheckMapCollisionUp(CollisionMapInfo& info)
{

	if (info.move.y <= 0)
	{
		return;
	}
	std::array<KamataEngine::Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真上の当たり判定
	bool hit = false;
	// 左上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) 
	{
		hit = true;
	}

	// 右上の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock)
	{
		hit = true;
	}

	if (hit)
	{
		// 盛り込みを排除する方向に移動量
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + KamataEngine::Vector3(0, +kHeight / 2.0f, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + KamataEngine::Vector3(0, +kHeight / 2.0f, 0));

		if (indexSetNow.yIndex != indexSet.yIndex)
		{
			// めり込み先ブロックの範囲矩形
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
			// 天井に当たった記録
			info.ceiling = true;
		}
	}
}

void P_Bullet::CheckMapCollisionDown(CollisionMapInfo& info) 
{
	if (info.move.y >= 0) 
	{
		return;
	}

	std::array<KamataEngine::Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真下の当たり判定
	bool hit = false;
	// 左下の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) 
	{
		hit = true;
	}

	// 右下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock)
	{
		hit = true;
	}

	// ブロックにヒット?
	if (hit) 
	{
		// 盛り込みを排除する方向に移動量
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + KamataEngine::Vector3(0, -kHeight / 2.0f, 0));
		// 現在座標が壁の外か
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + KamataEngine::Vector3(0, -kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex)
		{

			// めり込み先ブロックの範囲矩形
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
			// 天井に当たった記録
			info.landing = true;
		}
	}
}

void P_Bullet::CheckMapCollisionRight(CollisionMapInfo& info) 
{
	// 右移動アリ
	if (info.move.x <= 0)
	{
		return;
	}

	std::array<KamataEngine::Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真右の当たり判定
	bool hit = false;
	// 右上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock)
	{
		hit = true;
	}

	// 右下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock)
	{
		hit = true;
	}

	// ブロックにヒット?
	if (hit)
	{
		// 盛り込みを排除する方向に移動量
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + KamataEngine::Vector3(+kWidth / 2.0f, 0, 0));

		// 現在座標が壁の外か
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + KamataEngine::Vector3(+kWidth / 2.0f, 0, 0));

		if (indexSetNow.xIndex != indexSet.xIndex)
		{
			// めり込み先ブロックの範囲矩形
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.x = max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
			// 壁に当たった記録
			info.hitRightWall = true;
		}
	}
}

void P_Bullet::CheckMapCollisionLeft(CollisionMapInfo& info)
{
	// 左移動アリ
	if (info.move.x >= 0) 
	{
		return;
	}

	std::array<KamataEngine::Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真左の当たり判定
	bool hit = false;
	// 左上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock)
	{
		hit = true;
	}

	// 左下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) 
	{
		hit = true;
	}

	// ブロックにヒット?
	if (hit)
	{
		// 盛り込みを排除する方向に移動量
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + KamataEngine::Vector3(-kWidth / 2.0f, 0, 0));
		// 現在座標が壁の外か
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + KamataEngine::Vector3(-kWidth / 2.0f, 0, 0));

		if (indexSetNow.xIndex != indexSet.xIndex)
		{
			// めり込み先ブロックの範囲矩形
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.x = min(0.0f, rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));
			// 壁に当たった記録
			info.hitLeftWall = true;
		}
	}
}

// 弾の反射処理
void P_Bullet::CheckMapHit(CollisionMapInfo& info)
{

	// X軸反射
	if (info.hitLeftWall)
		velocity_.x = std::abs(velocity_.x);
	if (info.hitRightWall)
		velocity_.x = -std::abs(velocity_.x);

	// Y軸反射
	if (info.ceiling)
		velocity_.y = -std::abs(velocity_.y);
	if (info.landing)
		velocity_.y = std::abs(velocity_.y);

	reflection_ = true;
}

#pragma endregion

void P_Bullet::Draw() 
{
	if (!isActive_)
	{
		return;
	}

	model_->Draw(worldTransform_, *camera_);
}

#pragma region プレイヤーの弾と敵の衝突

AABB P_Bullet::GetAABB()
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 弾と敵の衝突応答
void P_Bullet::OnCollition(const Enemy* enemy) { (void)enemy; }

#pragma endregion

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

KamataEngine::Vector3 P_Bullet::CornerPosition(const KamataEngine::Vector3& center, Corner corner)
{

	KamataEngine::Vector3 offetTable[kNumCorner] = 
	{
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };

	return center + offetTable[static_cast<uint32_t>(corner)];
}