#pragma once

#include "KamataEngine.h"
#include "MyMath.h"
#include <list>


class P_Bullet;
class Enemy 
{
public:
	// デスフラグ
	bool isenemyDead_ = false;
	bool isenemyDead2_ = false;
    bool iscollition = false;
	// デスフラグのgetter
	// bool IsEnemyDead() const { return isenemyDead_; }

	// 体力表示
	int32_t enemyHp;

	// 敵の当たり判定サイズ
	static inline const float kWidth = 10.0f;
	static inline const float kHeight = 10.0f;

	// 歩行の速さ
	static inline const float kWalkSpeed = 0.04f;

	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = 5.0f;

	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 5.0f;

	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 5.0f;

	// 経過時間
	float walkTimer_ = 0.0f;

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();
	
#pragma region プレイヤーの弾と敵
	// AABBを取得
	AABB GetAABB();
	// 衝突応答
	void OnCollition(const P_Bullet* playerBullet);
#pragma endregion

	
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	int E_GetHP() const { return hp_; }
	int E_GetMaxHP() const { return maxHP_; }
	bool IsEnemyDead() const { return isenemyDead_; }
	


private:
	

	// ラ/ンダムで行動するためのタイマー
	float actionTimer_ = 0.0f;
	float nextActionTime_ = 0.0f;
	float setEnemy_ = 0.0f;

	int32_t respawnTimer = 80;

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Camera* camera_;
	// テクスチャハンドル
	// uint32_t textureHandle_ = 0u;

	KamataEngine::Model* model_;

	KamataEngine::Vector3 velocity_ = {};


	int32_t point = 0;
	int32_t maxP_ = 50000;

	
	int32_t maxHP_ = 10000;
	int32_t hp_ = maxHP_;
};