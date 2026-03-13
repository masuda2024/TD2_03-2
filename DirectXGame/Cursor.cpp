#include "Cursor.h"
#include"cassert"
#include "MapChipField.h"
#include <algorithm>
#include <list>
#include <numbers>
#define NOMINMAX

using namespace KamataEngine;
using namespace MathUtility;

KamataEngine::Vector3 Cursor::GetWorldPosition() 
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Cursor::Initialize(Model* model, Camera* camera, KamataEngine::Vector3& position)
{
	// NULLポイントチェック
	assert(model);

	model_ = model;

	// textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;

	camera_ = camera;

}

void Cursor::Update()
{

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	
	//マウスのカーソル位置切り替え
	ImGui::Text("Mouse Control: %s", ON_Mouse ? "ON" : "OFF");
	if (Input::GetInstance()->TriggerKey(DIK_M))
	{
		ON_Mouse = !ON_Mouse;  // 押すたびに反転
		OFF_Mouse = !ON_Mouse; // 逆状態にする
	}


	if (OFF_Mouse)
	{
		// キャラクターの移動速度
		const float kCursorSpeed = 0.5f;

		Vector3 acceleration = {};

		// 押した方向で移動ベクトルを変更(左右)
		if (Input::GetInstance()->PushKey(DIK_A))
		{
			move.x -= kCursorSpeed;
		} else if (Input::GetInstance()->PushKey(DIK_D))
		{
			move.x += kCursorSpeed;
			if (velocity_.x < 0.0f)
			{
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAccleration;
		}
		// 押した方向で移動ベクトルを変更(上下)
		if (Input::GetInstance()->PushKey(DIK_W))
		{
			move.y += kCursorSpeed;
		} else if (Input::GetInstance()->PushKey(DIK_S))
		{
			move.y -= kCursorSpeed;
		}
	}
	if (ON_Mouse)
	{
		
		

		Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();

		worldTransform_.translation_.x += mouseMove.lX * 0.15f;
		worldTransform_.translation_.y -= mouseMove.lY * 0.15f;
	
	}
	
	

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	// プレイヤーの座標の計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Cursor::Draw() { model_->Draw(worldTransform_, *camera_); }