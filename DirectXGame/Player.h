#pragma once
#include "KamataEngine.h"
#define NOMINMAX
#include <algorithm>
#include <cassert>
#include <list>
#include <numbers>

#include "MyMath.h"
#include "math.h"
#include <cmath>

#include "P_Bullet.h"

using namespace KamataEngine;
using namespace MathUtility;

class E_Bullet;
class Recovery;
class Player 
{
public:
	// キーボード入力
	KamataEngine::Input* input_ = nullptr;

	// 弾
	std::list<P_Bullet*> p_bullets_;

	// カーソル
	KamataEngine::Model* modelCursor_ = nullptr;

#pragma region 基本構成
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	~Player();
#pragma endregion

	void RotateX();
	void RotateZ();

	void Attack();

#pragma region プレイヤーの状態

	// 体力表示
	int32_t P_GetHP() const { return hp_; }
	int32_t P_GetMaxHP() const { return maxHP_; }
	bool IsDead() const { return isDead_; }
	// デスフラグ
	bool isDead_ = false;

#pragma endregion

#pragma region 衝突判定 [ プレイヤー  <<===>>  敵の弾 ]

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.5f;
	static inline const float kHeight = 1.0f;

	const std::list<P_Bullet*>& GetBullets() const { return p_bullets_; }

	AABB2 GetAABB2();
	void OnCollition2(const E_Bullet* enemybullet);
#pragma endregion

#pragma region 衝突判定 [ プレイヤー  <<===>>  回復アイテム ]

	AABB3 GetAABB3();
	void OnCollition3(const Recovery* recovery);

#pragma endregion

#pragma region 座標(ワールド変換)

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	const KamataEngine::Vector3& GetRotation() const { return worldTransform_.rotation_; }

#pragma endregion

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// 3Dレティクル用ワールド変換
	WorldTransform worldTransform3DReticle_;

	// カメラ
	KamataEngine::Camera* camera_;

	KamataEngine::Vector3 velocity_ = {};

	int32_t maxHP_ = 10000;
	int32_t hp_ = maxHP_;

	uint32_t HP_count = 0;


	// マウスの切り替え
	uint32_t OFF_Mouse = true;
	uint32_t ON_Mouse = false;


	//弾の発射音
	//uint32_t P_shotHandle_ = 0;
	//uint32_t P_shotSound_ = 0;
};
