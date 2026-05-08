#pragma once
#include"KamataEngine.h"
#include"Player.h"



class Player;
class Recovery 
{
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);
	void Update();
	void Draw();



	// 当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

#pragma region プレイヤーと回復アイテム
	// AABBを取得
	AABB3 GetAABB3();
	// 衝突応答
	void OnCollition3(const Player* player);
#pragma endregion



	
	// フラグ
	bool isDelete_recovery_ = false;
	bool IsDelete_Recovery() const { return isDelete_recovery_; }




private:

	
	
	KamataEngine::Model* model_;
	KamataEngine::Camera* camera_;
	KamataEngine::WorldTransform worldTransform_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};
