#pragma once
#include "KamataEngine.h"
#include"MyMath.h"

class MapChipField;

class Cursor
{
public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	// 敵の当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// マップとの当たり判定情報
	struct CollisionMapInfo 
	{
		bool ceiling = false; // 天井衝突フラグ
		bool langing = false; // 着地フラグ
		bool hitwall = false; // 壁接触フラグ
		                      // KamataEngine::Vector3 move = {}; // 移動量
	};
	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	// 加速度
	static inline const float kAccleration = 0.1f;
	// 減衰(ブレーキ)
	static inline const float kAttenuation = 0.5f;
	// 制限速度
	static inline const float kLimitRunSpeed = 0.25f;

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	const KamataEngine::Vector3& GetRotation() const { return worldTransform_.rotation_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Camera* camera_;
	// テクスチャハンドル
	// uint32_t textureHandle_ = 0u;

	KamataEngine::Model* model_;

	KamataEngine::Vector3 velocity_ = {};

	MapChipField* mapChipField_ = nullptr;

	//マウスの切り替え
	uint32_t OFF_Mouse = true;
	uint32_t ON_Mouse = false;

	
};
