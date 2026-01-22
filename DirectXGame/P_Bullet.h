#pragma once
#include "KamataEngine.h"
#include"MyMath.h"

class Enemy;

class Player;

class P_Bullet
{
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, Player* player);

	void Update();

	void StartAttack();

	void Draw();


	
	 // 有効かどうか
	bool IsActive() const { return isActive_; }




	// 終了フラグ
	bool isFinished_ = false;

	// デスフラグ
	bool isPBDead_ = false;

	bool IsPBDead() const { return isPBDead_; }

	// 速度
	KamataEngine::Vector3 velocity_ = {};

	// 当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();




	float angle_ = 0.0f;
	float cosValue_ = 0.0f;
	float sinValue_ = 0.0f;
	




#pragma region プレイヤーの弾と敵の衝突

	// AABBを取得
	AABB GetAABB();
	// 衝突応答
	void OnCollition(const Enemy* enemy);

#pragma endregion
	
	/*
	void SetEnemy(bool enemy) { isEnemy_ = enemy; }
	void SetEnemyPosition(const KamataEngine::Vector3& pos) { enemyPosition_ = pos; }
	const KamataEngine::Vector3& GetGrabPosition() const { return enemyPosition_; }
	
	bool IsEnemy() const { return isEnemy_; }
*/
private:

	// 攻撃のON/OFF
	bool isActive_ = false;
	Player* player_ = nullptr;
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_;

	// カメラ
	KamataEngine::Camera* camera_;

	// 速度
	KamataEngine::Vector3 Bulletvelocity_;


	float timer_ = 0.0f;
	static inline const float kLifeTime = 3.0f;
	
	//bool isEnemy_ = false;
	//KamataEngine::Vector3 enemyPosition_;
};
