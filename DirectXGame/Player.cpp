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

#pragma region 行列計算

// 行列の積
Matrix4x4 Multiply2(const Matrix4x4& m1, const Matrix4x4& m2)
{
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

	result.m[3][0] = {(m1.m[3][0] * m2.m[0][0]) + (m1.m[3][1] * m2.m[1][0]) + (m1.m[3][2] * m2.m[2][0]) + (m1.m[3][3] * m2.m[3][0])};
	result.m[3][1] = {(m1.m[3][0] * m2.m[0][1]) + (m1.m[3][1] * m2.m[1][1]) + (m1.m[3][2] * m2.m[2][1]) + (m1.m[3][3] * m2.m[3][1])};
	result.m[3][2] = {(m1.m[3][0] * m2.m[0][2]) + (m1.m[3][1] * m2.m[1][2]) + (m1.m[3][2] * m2.m[2][2]) + (m1.m[3][3] * m2.m[3][2])};
	result.m[3][3] = {(m1.m[3][0] * m2.m[0][3]) + (m1.m[3][1] * m2.m[1][3]) + (m1.m[3][2] * m2.m[2][3]) + (m1.m[3][3] * m2.m[3][3])};

	return result;
}

Matrix4x4 Inverse2(const Matrix4x4& m)
{
	Matrix4x4 result = {};
	float determinant =
	    m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
	    m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] - m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
	    m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
	    m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
	    m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
	    m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] + m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	if (determinant == 0.0f) 
	{
		return result;
	}

	float InDit = 1.0f / determinant;

	result.m[0][0] = InDit * (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
	                          m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);
	result.m[0][1] = InDit * (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
	                          m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
	result.m[0][2] = InDit * (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
	                          m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
	result.m[0][3] = InDit * (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
	                          m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);
	result.m[1][0] = InDit * (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
	                          m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
	result.m[1][1] = InDit * (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
	                          m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
	result.m[1][2] = InDit * (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
	                          m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
	result.m[1][3] = InDit * (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
	                          m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);
	result.m[2][0] = InDit * (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
	                          m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);
	result.m[2][1] = InDit * (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
	                          m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
	result.m[2][2] = InDit * (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
	                          m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
	result.m[2][3] = InDit * (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
	                          m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);
	result.m[3][0] = InDit * (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
	                          m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
	result.m[3][1] = InDit * (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
	                          m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);
	result.m[3][2] = InDit * (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
	                          m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
	result.m[3][3] = InDit * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
	                          m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);
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
	Matrix4x4 result = Multiply2(Multiply2(rotZ, rotX), rotY);

	return result;
}

#pragma endregion

void Player::Initialize(Model* model, Camera* camera, KamataEngine::Vector3& position)
{
	// NULLポイントチェック
	assert(model);

	model_ = model;

	// textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	camera_ = camera;

	// カーソル
	modelCursor_ = Model::CreateFromOBJ("Cursor", true);

	// 3Dレティクルのワールド変換を初期化
	worldTransform3DReticle_.Initialize();
	worldTransform3DReticle_.translation_ = position;
	worldTransform3DReticle_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	// シングルトンインスタンスを取得する
	input_ = KamataEngine::Input::GetInstance();





	//P_shotHandle_ = Audio::GetInstance()->LoadWave("Sounds/sound/Shot.mp3");


}

void Player::Update() 
{
	Attack();
#pragma region プレイヤーの移動

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A))
	{
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) 
	{
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_W)) 
	{
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) 
	{
		move.y -= kCharacterSpeed;
	}

	// 移動限界座標
	const float kMoveLimitX = 30.0f;
	const float kMoveLimitY = 20.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

#pragma endregion

#pragma region プレイヤーの攻撃
	;

	for (P_Bullet* p_bullet : p_bullets_) 
	{
		p_bullet->Update();
	}

	p_bullets_.remove_if([](P_Bullet* p_bullet)
		{
		if (p_bullet->IsDead_PB())
		{
			delete p_bullet;
			return true;
		}
		return false;
	});

#pragma endregion

#pragma region 3Dレティクルのモード切替

	if (Input::GetInstance()->TriggerKey(DIK_M))
	{
		ON_Mouse = (ON_Mouse == false);
		OFF_Mouse = (ON_Mouse == false);
	}

	if (OFF_Mouse)
	{

#pragma region カーソルの位置
		// プレイヤーから3Dレティクルへの距離
		const float kDistanceP_To_3DR = 50.0f;
		// プレイヤーから3Dレティクルへのオフセット(X+1向き)
		Vector3 offset = {0, 0, 1.0f};
		// プレイヤーのワールド変換の回転を反映
		offset = TransformNormal(offset, worldTransform_.matWorld_);
		// ベクトルの長さを整える
		offset = Normalize(offset) * kDistanceP_To_3DR;

		worldTransform3DReticle_.translation_ += move;
		// 位置
		worldTransform3DReticle_.translation_ = worldTransform_.translation_ + offset;

#pragma endregion
	}

	if (ON_Mouse) 
	{

#pragma region カーソルをマウス座標に合わせる

		POINT mousePos;
		GetCursorPos(&mousePos);

		HWND hwnd = KamataEngine::WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePos);

		// 画面サイズ
		float screenWidth = 1280.0f;
		float screenHeight = 720.0f;

		// NDC変換（-1～1）
		float ndcX = (mousePos.x / screenWidth) * 2.0f - 1.0f;
		float ndcY = 1.0f - (mousePos.y / screenHeight) * 2.0f;

		// カメラ空間
		float tanFov = tanf(camera_->fovAngleY * 0.5f);

		Vector3 rayDirCamera;
		rayDirCamera.x = ndcX * camera_->aspectRatio * tanFov;
		rayDirCamera.y = ndcY * tanFov;
		rayDirCamera.z = 1.0f; // ←重要

		rayDirCamera = Normalize(rayDirCamera);

		// カメラ回転
		Matrix4x4 rotMat = MakeRotateMatrix(camera_->rotation_);

		// ワールド方向
		Vector3 rayDir = TransformNormal(rayDirCamera, rotMat);
		rayDir = Normalize(rayDir);

		// カメラ位置
		Vector3 camPos = camera_->translation_;

		// レティクル位置
		const float kDistance = 50.0f;
		worldTransform3DReticle_.translation_ = camPos + rayDir * kDistance;

		// 行列更新
		worldTransform3DReticle_.TransferMatrix();

#pragma endregion
	}

#pragma endregion

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	//右クリックで回避
	if (Input::GetInstance()->IsPressMouse(1))
	{
		worldTransform_.translation_ += move * 5;
	}

#pragma region 回復


	if (hp_ > maxHP_)
	{
		hp_ = maxHP_;
	}

	if (Input::GetInstance()->TriggerKey(DIK_R) && HP_count == 0) 
	{
		hp_ += 10000;
		HP_count++;

		
	}

	if (Input::GetInstance()->TriggerKey(DIK_R) && HP_count == 2) 
	{
		hp_ += 10000;
		HP_count++;

		
	}

	if (Input::GetInstance()->TriggerKey(DIK_R) && HP_count == 4)
	{
		hp_ += 10000;
		HP_count++;

		
	}

#pragma region

	// 行列更新
	worldTransform3DReticle_.matWorld_ = MakeAffineMatrix(worldTransform3DReticle_.scale_, worldTransform3DReticle_.rotation_, worldTransform3DReticle_.translation_);
	worldTransform3DReticle_.TransferMatrix();

	// アフィン変換行列
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() 
{
	if (isDead_) 
	{
		return;
	}

	model_->Draw(worldTransform_, *camera_);

	for (P_Bullet* p_bullet : p_bullets_) 
	{
		p_bullet->Draw(*camera_);
	}

	modelCursor_->Draw(worldTransform3DReticle_, *camera_);
}

Player::~Player() 
{
	// 弾の解放
	for (P_Bullet* p_bullet : p_bullets_)
	{
		delete p_bullet;
	}

	delete modelCursor_;
}

#pragma region プレイヤーの動き

void Player::RotateX() 
{
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.007f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W))
	{
		worldTransform_.rotation_.x -= kRotSpeed;
	} else if (input_->PushKey(DIK_S))
	{
		worldTransform_.rotation_.x += kRotSpeed;
	}
}

void Player::RotateZ() 
{
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed2 = 0.01f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) 
	{
		worldTransform_.rotation_.z -= kRotSpeed2;
	} else if (input_->PushKey(DIK_S)) 
	{
		worldTransform_.rotation_.z += kRotSpeed2;
	}
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE) || input_->IsTriggerMouse(0))
	{
		//P_shotSound_ = Audio::GetInstance()->PlayWave(P_shotHandle_, true);

		const float kBulletSpeed = 1.0f;

		Vector3 velocity(kBulletSpeed, 0, 0);

		// velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		velocity = worldTransform3DReticle_.translation_ - worldTransform_.translation_;
		velocity = Normalize(velocity) * kBulletSpeed;

		// 弾生成
		P_Bullet* newBullet = new P_Bullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		p_bullets_.push_back(newBullet);
	}
}

#pragma endregion

#pragma region 衝突判定 [ プレイヤー  <<===>>  敵の弾 ]

KamataEngine::Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB2 Player::GetAABB2() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB2 aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 衝突応答
void Player::OnCollition2(const E_Bullet* enemyBullet)
{
	(void)enemyBullet;
	hp_ -= 500;
	if (hp_ <= 0) 
	{
		hp_ = 0;
		isDead_ = true;
	}
}
#pragma endregion

#pragma region 衝突判定 [ プレイヤー  <<===>>  回復アイテム ]
AABB3 Player::GetAABB3()
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB3 aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}
void Player::OnCollition3(const Recovery* recovery) 
{
	(void)recovery;
	hp_ += 100;
}

#pragma endregion