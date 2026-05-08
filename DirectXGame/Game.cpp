#include "Game.h"
#include "MapChipField.h"
#include "MyMath.h"
#include <random>

using namespace KamataEngine;
using namespace MathUtility;




#pragma region 行列計算

// 行列の積
Matrix4x4 Multiply3(const Matrix4x4& m1, const Matrix4x4& m2)
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

Matrix4x4 Inverse3(const Matrix4x4& m)
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

Matrix4x4 MakeRotateMatrix3(const Vector3& rotation)
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
	Matrix4x4 result = Multiply3(Multiply3(rotZ, rotX), rotY);

	return result;
}

#pragma endregion




#pragma region
#pragma endregion

void Game::Initialize()
{
	



#pragma region フェーズ・フェード
	// フェーズインから開始
	phase_ = Phase::kFadeIn;
	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
#pragma endregion

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	worldTransformEarth_.Initialize();
	worldTransformEarth_.translation_ = {0, -350, 100};

#pragma region 天球

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	modelEarth_ = KamataEngine::Model::CreateFromOBJ("earth", true);
	modelMoon_ = KamataEngine::Model::CreateFromOBJ("moon", true);

#pragma endregion

#pragma region 回復アイテム

	modelRecovery_ = KamataEngine::Model::CreateFromOBJ("kaifuku", true);




	recovery_ = new Recovery();
	KamataEngine::Vector3 recoveryPosition = {-35, 17, 0};
	recovery_->Initialize(modelRecovery_, &camera_, recoveryPosition);


	recovery2_ = new Recovery();
	KamataEngine::Vector3 recoveryPosition2 = {-31, 17, 0};
	recovery2_->Initialize(modelRecovery_, &camera_, recoveryPosition2);


	recovery3_ = new Recovery();
	KamataEngine::Vector3 recoveryPosition3 = {-27, 17, 0};
	recovery3_->Initialize(modelRecovery_, &camera_, recoveryPosition3);




#pragma endregion

#pragma region プレイヤー

	modelPlayer_ = Model::CreateFromOBJ("H_ziki", true);

	// プレイヤーの生成
	player_ = new Player();
	// プレイヤーの初期化
	KamataEngine::Vector3 playerPosition = {-20, 0, 0};
	player_->Initialize(modelPlayer_, &camera_, playerPosition);

#pragma endregion

#pragma region 敵
	modelEnemy_ = Model::CreateFromOBJ("kaizyu1", true);
	// 敵の生成
	enemy_ = new Enemy();
	// 敵の初期化
	KamataEngine::Vector3 enemyPosition = {40, 0, 0};
	enemy_->Initialize(modelEnemy_, &camera_, enemyPosition);
#pragma endregion

#pragma region カメラ関係

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(100, 200);

	// カメラの初期化
	camera_.Initialize();

	// カメラコントローラの初期化
	cameraController_ = new CameraController;
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

#pragma endregion


}


void Game::Update()
{

#pragma region デバッグカメラ

	// カメラコントロール
	cameraController_->Update();
	// デバッグカメラの更新
	debugCamera_->Update();

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0))
	{
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif // _DEBUG
	// ChangePhase();
	if (isDebugCameraActive_)
	{
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else 
	{
		camera_.TransferMatrix();
		camera_.UpdateMatrix();
	}

#pragma endregion

#ifdef _DEBUG
	// フェード
	fade_->Update();
	//ImGui::Text("C : Clear  ,  O : Over");
	//ImGui::Text("0(Zero) : DebugCamera ");
#endif 	
	


	worldTransformEarth_.matWorld_ = MakeAffineMatrix(worldTransformEarth_.scale_, worldTransformEarth_.rotation_, worldTransformEarth_.translation_);
	worldTransformEarth_.TransferMatrix();


	worldTransformEarth_.rotation_.z -= 0.0001f;







#pragma region UI

	// enemyHPのスプライト
	enemyHPHandle_ = TextureManager::Load("Sprits/Ehp.png");
	enemyHPSprite_ = KamataEngine::Sprite::Create(enemyHPHandle_, {1050, 0});

	_enemyHPHandle_ = TextureManager::Load("Sprits/Ehp_.png");
	_enemyHPSprite_ = KamataEngine::Sprite::Create(_enemyHPHandle_, {1050, 0});

	// 敵HP
	float enemyHpRatio = (float)enemy_->E_GetHP() / (float)enemy_->E_GetMaxHP();
	enemyHpRatio = std::clamp(enemyHpRatio, 0.0f, 1.0f);
	enemyHPSprite_->SetSize({enemyHpRatio * 300.0f, 30.0f}); // 幅200px、高さ20px
	enemyHPSprite_->SetPosition({980, 0});                   // 左上少し下に表示

	_enemyHPSprite_->SetSize({300.0f, 30.0f}); // 幅200px、高さ20px
	_enemyHPSprite_->SetPosition({980, 0});    // 左上少し下に表示

	playerHPHandle_ = TextureManager::Load("Sprits/hp.png");
	playerHPSprite_ = KamataEngine::Sprite::Create(playerHPHandle_, {0, 0});
	_playerHPHandle_ = TextureManager::Load("Sprits/hp_.png");
	_playerHPSprite_ = KamataEngine::Sprite::Create(_playerHPHandle_, {0, 0});

	// プレイヤーHP
	float hpRatio = (float)player_->P_GetHP() / (float)player_->P_GetMaxHP();
	hpRatio = std::clamp(hpRatio, 0.0f, 1.0f);
	playerHPSprite_->SetSize({hpRatio * 300.0f, 30.0f}); // 例：幅200px、高さ20px
	playerHPSprite_->SetPosition({0, 0});

	_playerHPSprite_->SetSize({300.0f, 30.0f}); // 例：幅200px、高さ20px
	_playerHPSprite_->SetPosition({0, 0});

#pragma endregion

#pragma region 回復アイテム
	

	
		recovery_->Update();
	
		recovery2_->Update();
	
		recovery3_->Update();
	
	
		if (Input::GetInstance()->TriggerKey(DIK_R))
		{
		    recovery = false;
		    R_count++;
	    }

		if (Input::GetInstance()->TriggerKey(DIK_R) && R_count == 2) 
		{
		    recovery2 = false;
		    R_count++;
	    }

		if (Input::GetInstance()->TriggerKey(DIK_R) && R_count == 4) 
		{
		    recovery3 = false;
		    R_count++;
	    }


#pragma endregion

#pragma region プレイヤー
	player_->Update();
	player_->RotateX();
	player_->RotateZ();
#pragma endregion

#pragma region 敵
		enemy_->Update();
#pragma endregion

#pragma region フェーズ

	switch (phase_) 
	{
	case Phase::kPlay:
		
		CheckAllCollisions();
#pragma region 仮設コード
		// ゲームクリア(仮)
		if (Input::GetInstance()->TriggerKey(DIK_C))
		{
			phase_ = Phase::kEnemyDeath;
		}
		// ゲームオーバー(仮)
		if (Input::GetInstance()->TriggerKey(DIK_O))
		{
			phase_ = Phase::kDeath;
		}

#pragma endregion

		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))
		{
			phase_ = Phase::kPose;
		}
		break;

	case Phase::kPose:


		//ImGui::Text("T : Title  ,  ESC : Continue");
		if (Input::GetInstance()->TriggerKey(DIK_T)) 
		{
			phase_ = Phase::kFadeOut3;
		}
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))
		{
			phase_ = Phase::kPlay;
		}



		break;

	case Phase::kDeath:

		// フェードアウト開始
		phase_ = Phase::kFadeOut;
		fade_->Start(Fade::Status::FadeOut, 1.0f);
		
		break;

	case Phase::kEnemyDeath:

		// フェードアウト開始
		phase_ = Phase::kFadeOut2;
		fade_->Start(Fade::Status::FadeOut, 1.0f);

		break;

	case Phase::kFadeIn:
		// フェード
		fade_->Update();
		if (fade_->IsFinished()) 
		{
			phase_ = Phase::kPlay;
		}
		break;


	case Phase::kFadeOut:
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			finishedGAME_ = true;
		}
		break;
	case Phase::kFadeOut2:
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			finishedGAME2_ = true;
		}
		break;

	case Phase::kFadeOut3:
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			finishedGAME3_ = true;
		}
		break;

	}
#pragma endregion



}

void Game::Draw()
{
	// スプライト
	Sprite::PreDraw();

#pragma region UI
	if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn || phase_ == Phase::kPose || phase_ == Phase::kDeath || phase_ == Phase::kEnemyDeath)
	{
		_playerHPSprite_->Draw();
		_enemyHPSprite_->Draw();

		playerHPSprite_->Draw();
		enemyHPSprite_->Draw();
	}
#pragma endregion

	Sprite::PostDraw();

	// モデル
	Model::PreDraw();

#pragma region 天球

	modelSkydome_->Draw(worldTransform_, camera_);
	modelEarth_->Draw(worldTransformEarth_, camera_);
	modelMoon_->Draw(worldTransform_, camera_);

#pragma endregion

#pragma region 回復アイテム
	
	
	if (recovery)
	{
		recovery_->Draw();
	}

	if (recovery2)
	{
		recovery2_->Draw();
	}
		
	if (recovery3)
	{
		recovery3_->Draw();
	}
	
#pragma endregion

#pragma region プレイヤー
	player_->Draw();
#pragma endregion

#pragma region 敵
	enemy_->Draw();
#pragma endregion

	Model::PostDraw();
}

Game::~Game()
{
	// フェードの解放
	delete fade_;

#pragma region UI

	delete playerHPSprite_;
	delete _playerHPSprite_;

	delete enemyHPSprite_;
	delete _enemyHPSprite_;

#pragma endregion

	delete modelSkydome_;
	delete modelEarth_;
	delete modelMoon_;
	
	delete recovery_;
	delete recovery2_;
	delete recovery3_;

	// プレイヤーの解放
	delete player_;
	// 敵の解放
	delete enemy_;

	// デバッグカメラの解放
	delete debugCamera_;
}

void Game::CheckAllCollisions()
{

	const std::list<P_Bullet*>& playerBullets = player_->GetBullets();
	const std::list<E_Bullet*>& enemyBullets = enemy_->GetE_Bullets();
	
#pragma region[ プレイヤーの弾  <<===>>  敵 ]

	AABB aabb1, aabb2;
	for (P_Bullet* p_bullet : playerBullets)
	{

		aabb1 = p_bullet->GetAABB();
		aabb2 = enemy_->GetAABB();
		if (IsCollition(aabb1, aabb2)) 
		{
			p_bullet->OnCollition(enemy_);
			enemy_->OnCollition(p_bullet);
		}
	}

#pragma endregion

#pragma region[ プレイヤー  <<===>>  敵の弾 ]

	AABB2 aabb3, aabb4;
	for (E_Bullet* e_bullet : enemyBullets)
	{

		aabb3 = e_bullet->GetAABB2();
		aabb4 = player_->GetAABB2();
		if (IsCollition2(aabb3, aabb4))
		{
			e_bullet->OnCollition2(player_);
			player_->OnCollition2(e_bullet);
		}
	}

#pragma endregion

#pragma region[ プレイヤー  <<===>>  回復アイテム ]

	AABB3 aabb5, aabb6;
	
		aabb5 = recovery_->GetAABB3();
		aabb6 = player_->GetAABB3();
		if (IsCollition3(aabb5, aabb6)) 
		{
			recovery_->OnCollition3(player_);
			player_->OnCollition3(recovery_);

			
		}
	
	
	
	

#pragma endregion

#pragma region[ プレイヤーの弾  <<===>>  敵の弾 ]

	AABB4 aabb7, aabb8;

	for (P_Bullet* p_bullet : playerBullets)
	{
		for (E_Bullet* e_bullet : enemyBullets)
		{

			aabb7 = p_bullet->GetAABB4();
			aabb8 = e_bullet->GetAABB4();
			if (IsCollition4(aabb7, aabb8))
			{
				p_bullet->OnCollition4(e_bullet);
				e_bullet->OnCollition4(p_bullet);
			}
		}
	}






#pragma endregion


}
