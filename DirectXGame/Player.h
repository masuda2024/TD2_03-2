#pragma once
#include "KamataEngine.h"
#include"MyMath.h"
#include"P_Bullet.h"
#include<list>
#include<DirectXMath.h>




class E_Bullet;
class MapChipField;
class Enemy;
class Player 
{
public:


#pragma region 基本構成
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	~Player();
#pragma endregion




#pragma region 敵の弾とプレイヤー
	// AABBを取得
	AABB2 GetAABB2();
	// 衝突応答
	void OnCollition2(const E_Bullet* enemyBullet);
#pragma endregion



	

#pragma region プレイヤーの状態
	// 体力表示
	//int32_t playerHp;
	int32_t P_GetHP() const { return hp_; }
	int32_t P_GetMaxHP() const { return maxHP_; }
	bool IsDead() const { return isDead_; }
	// デスフラグ
	bool isDead_ = false;
	// デスフラグのgetter
	// bool IsDead() const { return isDead_; }
	
	// Getter / 状態確認
	/**/
	int32_t GetHP() const { return hp_; }
	int32_t GetMaxHP() const { return maxHP_; }
	
	
	void RotateX();
	void RotateZ();


#pragma endregion

	
	
#pragma region プレイヤーと壁

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// 角
	enum Corner
	{
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数

	};

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);






#pragma endregion


#pragma region 動作




	// 加速度
	static inline const float kAccleration = 0.1f;
	// 減衰(ブレーキ)
	static inline const float kAttenuation = 0.5f;
	// 制限速度
	static inline const float kLimitRunSpeed = 0.25f;

	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.0f; // 0,1f
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.0f; // 0.5f
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 0.8f;
	// 着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.9f;
	// 微小な数値
	static inline const float kGroundSearchHeight = 0.1f;
	// 着地時の速度減衰率
	static inline const float kAttenuationWall = 0.9f;





	// 左右
	enum class LRDirection 
	{
		kRight,
		kLeft,
	};

	// 旋回開始時の角度
	float trunFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float trunTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	// 接地状態フラグ
	bool onGround_ = true;

	LRDirection lrDirection_ = LRDirection::kRight;
#pragma endregion

#pragma region 座標(ワールド変換)


	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	const KamataEngine::Vector3& GetRotation() const { return worldTransform_.rotation_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
#pragma endregion

#pragma region 回転

	bool prevOnGround_ = false;

	float angle_ = 0.0f;
	float cosValue_ = 0.0f;
	float sinValue_ = 0.0f;

	static inline const float kBlank = 0.9f;

	void AnimateTurn();

#pragma endregion

	
private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera* camera_;
	
	// テクスチャハンドル
	// uint32_t textureHandle_ = 0u;

	// モデル
	KamataEngine::Model* model_;

	KamataEngine::Vector3 velocity_ = {};

	MapChipField* mapChipField_ = nullptr;

	int32_t maxHP_ = 10000;
	int32_t hp_ = maxHP_;

	
};
