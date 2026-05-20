#include "Enemy.h"
#include"Player.h"
#include <algorithm>
#include <cassert>
#include <list>
#include <numbers>
#define NOMINMAX
#include "MyMath.h"
#include "math.h"

using namespace KamataEngine;
using namespace MathUtility;

void Enemy::Initialize(Model* model, Camera* camera, KamataEngine::Vector3& position, Player* player) 
{
	// NULLポイントチェック
	assert(model);

	model_ = model;

	// textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;

	camera_ = camera;

	player_ = player;


	ApproachInitialize();



	//効果音ラボ  戦闘[1] ドラゴンの鳴き声1 恐竜や怪獣の声にも使える
	E_V_Handle_ = Audio::GetInstance()->LoadWave("Sounds/sound/Dragon's_Roar1.mp3");


}

void Enemy::Update()
{
	// キャラクターの移動ベクトル
	// Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	// const float kCharacterSpeed = 0.2f;



#pragma region 敵の行動フェーズ


	
	


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
			// 50%の確率で撃つ
			if (rand() % 2 == 0) 
			{
				Fire();
			}
			// 発射タイマーを初期化
			fireTimer_ = kFireInterval;
		}


		// HPでフェーズ変更
		if (E_hp_ < E_maxHP_ * 0.3f && phase_ != Phase::Rage)
		{
			phase_ = Phase::Rage;
		}

		break;
	}
		
	case Phase::Rage:
		{


		// 咆哮
		if (isE_V_Played_ == 0) 
		{
			E_Voice_ = Audio::GetInstance()->PlayWave(E_V_Handle_, false);
			isE_V_Played_ = true;
		}


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

			///////////////////////////////////////////

			// プレイヤー方向
			Vector3 enemyPos = worldTransform_.translation_;
			Vector3 playerPos = player_->GetWorldPosition();

			Vector3 velocity;

			velocity.x = playerPos.x - enemyPos.x;
			velocity.y = playerPos.y - enemyPos.y;
			velocity.z = playerPos.z - enemyPos.z;

			// ベクトルの長さ
			float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);

			// 正規化
			velocity.x /= length;
			velocity.y /= length;
			velocity.z /= length;

			// 弾速
			const float kBulletSpeed = 1.0f;

			velocity.x *= kBulletSpeed;
			velocity.y *= kBulletSpeed;
			velocity.z *= kBulletSpeed;

			// 弾生成
			E_Bullet* new_e_Bullet = new E_Bullet();

			new_e_Bullet->Initialize(model_, worldTransform_.translation_, velocity);

			e_bullets_.push_back(new_e_Bullet);

			/////////////////////////////////////////////////////////////






			Fire();
			fireTimer_ = 10; // ←めっちゃ速くする
		}


		
		if (E_hp_ <= 0)
		{
			/// isEnemyDead_ = true;
			phase_ = Phase::Destroyed;
			
		}
		
		

		break;
		}
	case Phase::Destroyed:
		{

		
		worldTransform_.translation_.y -= 0.2f;
		worldTransform_.rotation_.z += 0.001f;
		worldTransform_.rotation_.x += 0.2f;

		clearTimer += 10;

		if (clearTimer==1000)
		{
			isEnemyDead_ = true;
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
	//if (isEnemyDead_)
	//{
	//	return;
	//}

	model_->Draw(worldTransform_, *camera_);

	for (E_Bullet* e_bullet : e_bullets_)
	{
		e_bullet->Draw(*camera_);
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
	/*
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	KamataEngine::Vector3 velocity(0, 0, kBulletSpeed);

	// 弾を生成し、初期化
	E_Bullet* new_e_Bullet = new E_Bullet();
	new_e_Bullet->Initialize(model_, worldTransform_.translation_, velocity);
	// 弾を登録する
	e_bullets_.push_back(new_e_Bullet);
*/


	/////////////
	// 弾の速度
	const float kBulletSpeed = 1.0f;

	// KamataEngine::Vector3 velocity(0, 0, kBulletSpeed);

	// 敵位置
	Vector3 enemyPos = worldTransform_.translation_;

	// プレイヤー位置
	Vector3 playerPos = player_->GetWorldPosition();

	// プレイヤー方向
	Vector3 velocity;

	velocity.x = playerPos.x - enemyPos.x;
	velocity.y = playerPos.y - enemyPos.y;
	velocity.z = playerPos.z - enemyPos.z;

	// 正規化
	float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);

	velocity.x /= length;
	velocity.y /= length;
	velocity.z /= length;

	// スピード掛ける
	velocity.x *= kBulletSpeed;
	velocity.y *= kBulletSpeed;
	velocity.z *= kBulletSpeed;

	// 弾を生成し、初期化
	E_Bullet* new_e_Bullet = new E_Bullet();
	new_e_Bullet->Initialize(model_, worldTransform_.translation_, velocity);
	// 弾を登録する
	e_bullets_.push_back(new_e_Bullet);
	/////////////
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
		//phase_ = Phase::Destroyed;
	}
}

#pragma endregion
