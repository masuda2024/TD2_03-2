#include "Enemy.h"

#include <algorithm>
#include <cassert>
#include <list>
#include <numbers>
#define NOMINMAX
#include "MyMath.h"
#include "math.h"

using namespace KamataEngine;
using namespace MathUtility;

void Enemy::Initialize(Model* model, Camera* camera, KamataEngine::Vector3& position)
{
	// NULLポイントチェック
	assert(model);

	model_ = model;

	// textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;

	camera_ = camera;

	ApproachInitialize();
}

void Enemy::Update()
{
	// キャラクターの移動ベクトル
	// Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	// const float kCharacterSpeed = 0.2f;

#pragma region 敵の行動フェーズ


	// HPでフェーズ変更
	if (E_hp_ < E_maxHP_ * 0.3f && phase_ != Phase::Rage)
	{
		phase_ = Phase::Rage;
	}


	// 敵の行動フェーズ
	switch (phase_)
	{
	case Phase::Approach:
		{
	default:
		// 移動(ベクトルを減算)
		worldTransform_.translation_.x -= 0.2f;
		if (worldTransform_.translation_.x < 30.0f) 
		{
			phase_ = Phase::Attack;
		}
		break;
		}
	case Phase::Attack: 
	{

#pragma region 敵の上下移動

		// 時間のカウンター
		walkTimer_ += 5.0f / 60.0f; // フレームごとの時間増分
		// 上下に揺れるように移動
		float amplitude = 20.0f; // 上下移動の幅（単位:座標）
		float speed = 0.1f;      // 速さ
		// Y方向にsinで移動
		worldTransform_.translation_.y = sin(walkTimer_ * speed) * amplitude;
		// X,Y をいじるので一旦変数に出す
		Vector3& pos = worldTransform_.translation_;
		pos.y = sin(walkTimer_ * 0.1f) * 10.0f;

#pragma endregion

		// 発射タイマーカウントダウン
		fireTimer_--;
		// 指定時間に達した
		if (fireTimer_ == 0)
		{
			// 弾を発射
			Fire();
			// 発射タイマーを初期化
			fireTimer_ = kFireInterval;
		}

		break;
	}
	case Phase::Rage:
		{

		// 移動スピードアップ
		worldTransform_.translation_.x -= 0.4f;

		if (worldTransform_.translation_.x < 20.0f)
		{
			worldTransform_.translation_.x = 20.0f;
		}

		// 上下移動激しく
		walkTimer_ += 10.0f / 60.0f;
		worldTransform_.translation_.y = sin(walkTimer_ * 0.2f) * 20.0f;


		// 発射間隔短くする
		fireTimer_--;
		if (fireTimer_ <= 0) 
		{
			Fire();
			fireTimer_ = 10; // ←めっちゃ速くする
		}

		break;
		}
	}

#pragma endregion

#pragma region 敵の攻撃
	// Fire();

	for (E_Bullet* e_bullet : e_bullets_)
	{
		e_bullet->Update();
	}

	e_bullets_.remove_if([](E_Bullet* e_bullet){
		if (e_bullet->IsDead_EB())
		{
			delete e_bullet;
			return true;
		}
		return false;
	});

#pragma endregion

	// アフィン変換行列
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Enemy::Draw()
{
	if (isEnemyDead_)
	{
		return;
	}

	model_->Draw(worldTransform_, *camera_);

	for (E_Bullet* e_bullet : e_bullets_)
	{
		e_bullet->Draw(*camera_);
	}

	if (E_hp_ < 0)
	{
		isEnemyDead_ = true;
	}
}

Enemy::~Enemy()
{
	for (E_Bullet* e_bullet : e_bullets_)
	{
		delete e_bullet;
	}
}

void Enemy::Fire() 
{
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	KamataEngine::Vector3 velocity(0, 0, kBulletSpeed);

	// 弾を生成し、初期化
	E_Bullet* new_e_Bullet = new E_Bullet();
	new_e_Bullet->Initialize(model_, worldTransform_.translation_, velocity);
	// 弾を登録する
	e_bullets_.push_back(new_e_Bullet);
}

void Enemy::ApproachInitialize()
{
	// 発射タイマーを初期化
	fireTimer_ = kFireInterval;
}

#pragma region 衝突判定 [ プレイヤーの弾  <<===>>  敵 ]

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

#pragma endregion
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
	E_hp_ -= 100;
	if (E_hp_ <= 0) 
	{
		E_hp_ = 0;
		isEnemyDead_ = true;
	}
}