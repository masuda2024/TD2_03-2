#include"Player.h"
#include"Cursor.h"
#include"KamataEngine.h"
#include"Game.h"
#include"cassert"
#include<numbers>
#include<algorithm>
#include<list>
#define NOMINMAX



using namespace KamataEngine;
using namespace MathUtility;


/*
float Length(const Vector3& v)
{
	float result;
	result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return result;
};


Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};

	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	result.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	result.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

	result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	result.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	result.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

	result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	result.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	result.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

	result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	result.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	result.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

	return result;
}
// ビューボート変換行列
Matrix4x4 MakeViewportMatrix(float VpWidth, float VpHeight, float OffsetX, float OffsetY) {

	Matrix4x4 result{};

	result.m[0][0] = VpWidth / 2.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = -(VpHeight / 2.0f);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = (VpWidth / 2.0f) + OffsetX;
	result.m[3][1] = (VpHeight / 2.0f) + OffsetY;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}


*/


void Player::Initialize(Model* model, Camera* camera, KamataEngine::Vector3& position) 
{
	// NULLポイントチェック
	assert(model);

	model_ = model;

	//3Dレティクルのワールドトランスフォーム初期化
	//worldTransform3DReticle_.Initialize();

	// textureHandle_ = textureHandle;
    worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	camera_ = camera;

	/*
	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("Cursor.png");
	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {0, 0}, {1, 1, 1, 1}, {50, 50});

	// カーソル
	modelCursor_ = Model::CreateFromOBJ("Cursor", true);
*/


	
	//Matrix4x4 viewport;
}

void Player::Update()
{
	



	/*
	// 自機から3Dレティクルへの距離
	const float kDistanceP_to_3DR = 50.0f;
	
	// 前方向ベクトル(X+向き)
	Vector3 offset = {1.0f, 0, 0};
	
	//自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);

	//ベクトルの長さを整える
	offset = Normalize(offset) * kDistanceP_to_3DR;

	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = worldTransform_.translation_ + offset;
	
	//worldTransform3DReticle_のワールド行列更新と転送
	worldTransform3DReticle_.rotation_ = worldTransform_.rotation_;
*/



	

	
	/*
	

	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 viewportMatrix;
	


	// マウス座標(スクリーン座標)を取得する
	POINT mousePoint;
	GetCursorPos(&mousePoint);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePoint);
	



	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = Multiply(Multiply(viewMatrix, projectionMatrix), viewportMatrix);
	
	// 合成行列の逆行列
	Matrix4x4 matInverceVPV = Inverse(matVPV);
	
	// スクリーン座標
	KamataEngine::Vector3 posNear = KamataEngine::Vector3(static_cast<float>(mousePoint.x), static_cast<float>(mousePoint.y), 0.0f);
	KamataEngine::Vector3 posFar = KamataEngine::Vector3(static_cast<float>(mousePoint.x), static_cast<float>(mousePoint.y), 1.0f);
	
	// スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverceVPV);
	posFar = Transform(posFar, matInverceVPV);
	
	// マウスレイの方向
	KamataEngine::Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Normalize(mouseDirection);
	
	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;
	worldTransform3DReticle_.translation_ = posNear + mouseDirection * kDistanceTestObject;
	//worldTransform3DReticle_のワールド行列更新と転送
	



*/






	// 7.旋回制御
	AnimateTurn();

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速度
	const float kCharacterSpeed = 0.5f;

	Vector3 acceleration = {};

	// --- メンバ変数を更新 ---
	/*
	angle_ = worldTransform_.rotation_.x;
	cosValue_ = std::cos(angle_);
	sinValue_ = std::sin(angle_);

	constexpr float kTurnSpeed = 0.09f;
	worldTransform_.rotation_.x += kTurnSpeed;
	if (worldTransform_.rotation_.x > 6.2831853f) 
	{
		worldTransform_.rotation_.x -= 6.2831853f;
	}*/

	// 押した方向で移動ベクトルを変更(左右)
	if (Input::GetInstance()->PushKey(DIK_A)) 
	{
		move.x -= kCharacterSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_D))
	{
		move.x += kCharacterSpeed;
		if (velocity_.x < 0.0f) 
		{
			// 速度と逆方向に入力中は急ブレーキ
			velocity_.x *= (1.0f - kAttenuation);
		}
		acceleration.x += kAccleration;
		if (lrDirection_ != LRDirection::kRight)
		{
			lrDirection_ = LRDirection::kRight;
			// 旋回開始時の角度を記録する
			trunFirstRotationY_ = worldTransform_.rotation_.y;
			// 旋回タイマーに時間を設定する
			trunTimer_ = kTimeTurn;
		}
	}
	// 押した方向で移動ベクトルを変更(上下)
	if (Input::GetInstance()->PushKey(DIK_W))
	{
		move.y += kCharacterSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_S)) 
	{
		move.y -= kCharacterSpeed;
	}









	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	// アフィン変換行列
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix(); // プレイヤーの座標の計算
}

// プレイヤーの描画(敵当たったら非表示になる)
void Player::Draw()
{
	if (isDead_) 
	{
		return;
	}
	
	// プレイヤーモデルの描画
	model_->Draw(worldTransform_, *camera_);
	/*
	// 3Dレティクルの描画
	if (modelCursor_) 
	{
		modelCursor_->Draw(worldTransform3DReticle_, *camera_);
	}*/
}

// 7.旋回制御
void Player::AnimateTurn() 
{
	// 旋回制御
	if (trunTimer_ > 0.0f) 
	{
		trunTimer_ -= 1.0f / 60.0f;

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, trunFirstRotationY_, trunTimer_ / kTimeTurn);
	}
}

Player::~Player() 
{ 
	delete sprite2DReticle_; 
	//delete cursor_;
}

#pragma region プレイヤーの弾と敵の衝突

KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner)
{
	KamataEngine::Vector3 offsetTable[kNumCorner] =
	{
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  //  kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

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

#pragma endregion