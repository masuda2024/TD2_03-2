#include "Game.h"
#include "MapChipField.h"
#include "MyMath.h"

#include <random>
#include <cstdlib>
#include <ctime>

#include "CameraController.h"
#include "Fade.h"

#include "Player.h"
#include "P_Bullet.h"

#include "Enemy.h"



#include "imgui.h"

#include "Windows.h"

#include"math.h"
//#include"Matrix4x4.h"
using namespace KamataEngine;



struct MousePosition
{
	float x;
	float y;
};

struct Vector2
{
	float x;
	float y;
};





#pragma region ベクトルの計算

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
	return result;
};

// 減算
Vector3 VectorSubtract(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	return result;
};

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v)
{
	Vector3 result;
	result = {v.x * scalar, v.y * scalar, v.z * scalar};
	return result;
};

// 内積
float Dot(const Vector3& v1, const Vector3& v2) 
{
	float result;
	result = {v1.x * v2.x + v1.y * v2.y + v1.z * v2.z};
	return result;
};

// 長さ(ノルム)
float Length(const Vector3& v)
{
	float result;
	result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return result;
};

// 正規化
Vector3 Nomalize(const Vector3& v)
{
	Vector3 result;
	result = {v.x / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z), v.y / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z), v.z / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z)};
	return result;
};

#pragma endregion


// ビューボート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{

	Matrix4x4 result{};

	result.m[0][0] = width / 2.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = -(height / 2.0f);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[2][3] = 0.0f;

	result.m[3][0] = left + (width / 2.0f);
	result.m[3][1] = top + (height / 2.0f);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1.0f;

	return result;
}

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
// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result{};

	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	result = Multiply(Multiply(scaleMatrix, rotateXYZMatrix), translateMatrix);

	return result;
}








#pragma region
#pragma endregion

void Game::Initialize()
{
#pragma region

#pragma endregion

	
	//マウスカーソルの位置
	//Input::GetInstance()->GetMousePosition();
	
	//Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();






	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();


	//ブロック
	modelBlock_ = Model::CreateFromOBJ("block");
	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	// マップチップフィールドの初期化
	mapChipField_->LoadMapchipCsv("Resources/blocks.csv");
	GenerateBlocks();
	
	

	P_Shot_ = Audio::GetInstance()->LoadWave("Sounds/sound/Shot.mp3");


	

	
#pragma region スカイドーム

	modelskydome_ = Model::CreateFromOBJ("SkyDome", true);
	// スカイドームの生成
	skydome_ = new Skydome();
	skydome_->Initialize(modelskydome_, textureHandle_, &camera_);

#pragma endregion
	
#pragma region プレイヤー

	
	// プレイヤー
	modelPlayer_ = Model::CreateFromOBJ("player", true);

	


	// プレイヤーの弾
	modelPlayerBullet_ = Model::CreateFromOBJ("bullet", true);
	// パーティクルの3Dモデルデータの生成
	model_P_Particle_ = Model::CreateFromOBJ("deathParticle", true);

	// プレイヤーの生成
	player_ = new Player();
	


	// プレイヤーの座標を指定
	//Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(6, 30);
	KamataEngine::Vector3 playerPosition = {-20, 0, 0};
	player_->Initialize(modelPlayer_, &camera_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	


	// プレイヤーの弾
	for (int i = 0; i < 7; i++)
	{
		P_Bullet* bullet = new P_Bullet();
		bullet->Initialize(modelPlayerBullet_, &camera_, player_);
		bullet->SetMapChipField(mapChipField_);
		bullets_.push_back(bullet);
	}

	// プレイヤーのデスパーティクル
	P_Particles_ = new P_DeathParticle();
	P_Particles_->Initialize(model_P_Particle_, &camera_, playerPosition);



	//得点
	pointHandle_ = TextureManager::Load("Point.png");
	pointSprite_ = KamataEngine::Sprite::Create(pointHandle_, {0, 0});
	//時間
	timeHandle_ = TextureManager::Load("Time.png");
	timeSprite_ = KamataEngine::Sprite::Create(timeHandle_, {0, 0});


#pragma endregion




#pragma region カーソル
	
	// カーソル
	modelCursor_ = Model::CreateFromOBJ("Cursor", true);
	
	// カーソル
	cursor_ = new Cursor();
	
	// カーソルの初期化
	KamataEngine::Vector3 cursorPosition = {15, 0, 0};
	//cursor_->Initialize(modelCursor_, &camera_, playerPosition);
	cursor_->Initialize(modelCursor_, &camera_, cursorPosition);
	
	//cursor_->SetMapChipField(mapChipField_);

#pragma endregion





#pragma region 敵

	
	// 敵の3Dモデル
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	
	model_E_Particle_ = Model::CreateFromOBJ("E_deathParticle", true);

    
	    // 敵座標をマップチップ番号で指定
	std::vector<KamataEngine::Vector2> enemyTilePositions = 
	{
	    {60, 15},
	    //{15, 11},
		//{25, 5},
		//{35, 8},
		//{45, 30},
		//{60, 10},
        //{75, 6 },
		//{5,17},
	    //{2, 25 },
	    //{27, 8 },
	};

	// 敵座標をマップチップ番号で指定
	for (const auto& tilePos : enemyTilePositions)
	{
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(static_cast<uint32_t>(tilePos.x), static_cast<uint32_t>(tilePos.y));
		Vector3 enemySize = {1.0f, 1.0f, 1.0f};
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);
		enemies_.push_back(newEnemy);

		// 敵のデスパーティクル
		E_Particles_ = new E_DeathParticle();
		E_Particles_->Initialize(model_E_Particle_, &camera_, enemyPosition);
	}


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

#pragma region フェーズ・フェード
	// フェーズインから開始
	phase_ = Phase::kFadeIn;
	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
#pragma endregion

}

void Game::GenerateBlocks()
{

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定
	worldTransformBlocks_.resize(42);
	for (uint32_t i = 0; i < 42; ++i) 
	{
		// 1列の要素数を設定
		worldTransformBlocks_[i].resize(100);
	}

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i)
	{
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) 
		{
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) 
			{
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}


void Game::Update()
{
	// フェード
	fade_->Update();
	
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 viewportMatrix;
	





#pragma region マウスカーソル位置
	//マウス座標(スクリーン座標)を取得する
	POINT mousePoint;
	GetCursorPos(&mousePoint);
	//クライアントエリア座標に変換する
	//HWND hwnd = GetActiveWindow();
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePoint);
	//マウス座標をモデル座標に変換する
	



	//ビュープロジェクションビューポート合成行列
    Matrix4x4 matVPV = Multiply(Multiply(viewMatrix, projectionMatrix), viewportMatrix);
	//合成行列の逆行列
    Matrix4x4 matInverceVPV = Inverse(matVPV);
	//スクリーン座標
	KamataEngine::Vector3 posNear = KamataEngine::Vector3(static_cast<float>(mousePoint.x), static_cast<float>(mousePoint.y), 0.0f);
	KamataEngine::Vector3 posFar = KamataEngine::Vector3(static_cast<float>(mousePoint.x), static_cast<float>(mousePoint.y), 1.0f);
	//スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverceVPV);
	posFar = Transform(posFar, matInverceVPV);
	// マウスレイの方向
	KamataEngine::Vector3 mouseDirection = posNear - posFar;
	mouseDirection = Normalize(mouseDirection);
	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;
	worldTransform_.translation_ = posNear + mouseDirection * kDistanceTestObject;
	







	int mouseX = mousePoint.x;
	int mouseY = mousePoint.y;
	

	ImGui::Text("mousePoint X  %d", mouseX);
	ImGui::Text("mousePoint Y  %d", mouseY);

	//cursor_->SetPosition({static_cast<float>(mouseX), static_cast<float>(mouseY)});
	//int MoveF = 0;

#pragma endregion
	
	













	/*
	KamataEngine::Vector2 current_P = {0.0f, 0.0f};

	for (P_Bullet* bullet : bullets_) 
	{
		float start = bullet->GetWorldPosition().x;
		float end = static_cast<float>(mouseX) / 10.0f; // マウスのX座標を10で割って弾の終点とする
		
		current_P.x = start + (end - start) * MoveF / 10.0f; // MoveFを10で割って0から1の範囲にする
		
		if (Cursor_ON_Mouse)
		{
			MoveF = 1;

		}

	}

*/

	/*
	
	




	if (Cursor_ON_Mouse)
	{
		cursor_->SetPosition({static_cast<float>(mouseX), static_cast<float>(mouseY)});
	}
*/
	




	time -= 20;

	float timeRatio = (float)time / (float)maxtime;
	timeRatio = std::clamp(timeRatio, 0.0f, 1.0f);
	timeSprite_->SetSize({timeRatio * 1280.0f, 30.0f}); // 幅200px、高さ20px
	timeSprite_->SetPosition({0, 0});           



	float scoreRatio = (float)score / (float)MaxScore;
	scoreRatio = std::clamp(scoreRatio, 0.0f, 1.0f);
	pointSprite_->SetSize({scoreRatio * 1280.0f, 30.0f}); // 幅200px、高さ20px
	pointSprite_->SetPosition({0, 30});           



	
	if (time <= 0) 
	{
		phase_ = Phase::kDeath;
	}

	if (score >= MaxScore)
	{
		phase_ = Phase::kEnemyDeath;
	}



	// 天球の更新
	skydome_->Update();
#pragma region プレイヤー

	player_->Update();
	
	// プレイヤーの攻撃を呼び出す
	

	if (Input::GetInstance()->IsTriggerMouse(0))
	{
		Audio::GetInstance()->PlayWave(P_Shot_);
		for (P_Bullet* bullet : bullets_)
		{
			if (!bullet->IsActive())
			{
				bullet->StartAttack();
				break;
			}
		}
	}



	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) 
	{
		Audio::GetInstance()->PlayWave(P_Shot_);
		for (P_Bullet* bullet : bullets_)
		{
			if (!bullet->IsActive()) 
			{
				bullet->StartAttack();
				break;
			}
		}
	}
	// プレイヤーの弾を更新
	for (P_Bullet* bullet : bullets_) 
	{
		bullet->Update();
		if (!bullet->GetReflection() && bullet->IsActive())
		{
			score += 200;
		} 
		//ImGui::Text("Score x 2 %d", bullet->GetReflection());
	}


#pragma endregion

#pragma region カーソル


	cursor_->Update();
	Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();
	ImGui::Text("Mouse Move X: %ld, Y: %ld, Z: %ld", mouseMove.lX, mouseMove.lY, mouseMove.lZ);


#pragma endregion

#pragma region 敵
	
	for (Enemy* enemy : enemies_)
	{
		enemy->Update();
	}
	
	
	//ImGui::Text("Score %d", score);

#pragma endregion

	// カメラコントロール
	cameraController_->Update();
	// デバッグカメラの更新
	debugCamera_->Update();

	switch (phase_)
	{
	case Phase::kPlay:

		// 全ての当たり判定
		CheckAllCollisions();

		if (Input::GetInstance()->TriggerKey(DIK_SPACE))
		{
			// Audio::GetInstance()->PlayWave(PBSound_);
		}

		// ゲームプレイフェーズの処理
		if (player_->IsDead() == true)
		{
			// デス演出フェーズに切り替え
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const KamataEngine::Vector3 deathParticlesPosition = player_->GetWorldPosition();

			// パーティクル
			P_Particles_ = new P_DeathParticle();
			P_Particles_->Initialize(model_P_Particle_, &camera_, deathParticlesPosition);
		}
		for (Enemy* enemy : enemies_)
		{


			if (enemy->IsEnemyDead() == true)
			{
				std::vector<KamataEngine::Vector2> enemyTilePositions;

				// 敵の座標を取得
				const KamataEngine::Vector3 E_deathParticlesPosition = enemy->GetWorldPosition();

				// パーティクル
				E_Particles_ = new E_DeathParticle();
				E_Particles_->Initialize(model_E_Particle_, &camera_, E_deathParticlesPosition);
			} 


			if (enemy->IsEnemyDead2() == true)
			{
				phase_ = Phase::kEnemyDeath;
			}
		}
		
		



		break;

	case Phase::kDeath:
		// デス演出フェーズの処理

		// デスパーティクルの更新
		P_Particles_->Update();
		if (P_Particles_ && P_Particles_->isFinished_) 
		{
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}

		break;

		
	
	case Phase::kEnemyDeath:

		// デスパーティクルの更新
		E_Particles_->Update();
		if (E_Particles_ && E_Particles_->isFinished_)
		{
			// フェードアウト開始
			phase_ = Phase::kFadeOut2;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
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
	}


	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine)
		{
			if (!worldTransformBlock)
				continue;
			// アフィン変換行列の作成
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}


#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) 
	{
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif // _DEBUG
	ChangePhase();
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
}


// フェーズ
void Game::ChangePhase()
{

	switch (phase_) 
	{
	case Phase::kPlay:
// ゲームプレイフェーズの処理
#pragma region プレイヤー
		if (player_->IsDead() == true)
		{
			// デス演出フェーズに切り替え
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const KamataEngine::Vector3 deathParticlesPosition = player_->GetWorldPosition();

			// パーティクル
			P_Particles_ = new P_DeathParticle();
			P_Particles_->Initialize(model_P_Particle_, &camera_, deathParticlesPosition);
		}
#pragma endregion

#pragma region 敵
		for (Enemy* enemy : enemies_)
		{
			if (enemy->IsEnemyDead2() == true)
			{
				// デス演出フェーズに切り替え
				phase_ = Phase::kEnemyDeath;
			}
		}
		
#pragma endregion

		break;

	case Phase::kDeath:
		// デス演出フェーズの処理

		if (P_Particles_) 
		{
			// シーン終了
			finishedGAME_ = true;
		}

		break;
	case Phase::kEnemyDeath:

		// シーン終了
		finishedGAME2_ = true;

		break;
	}
}


void Game::CheckAllCollisions()
{
#pragma region プレイヤーの弾と敵
	// 判定対象1と2の座標
	AABB aabb1, aabb2;
	
	

	

	for (P_Bullet* bullet : bullets_)
	{
		// プレイヤーの弾
		aabb1 = bullet->GetAABB();
		for (Enemy* enemy : enemies_)
		{
			aabb2 = enemy->GetAABB();
			if (IsCollition(aabb1, aabb2))
			{
				// 自キャラの衝突時関数を呼び出す
				bullet->OnCollition(enemy);
				enemy->OnCollition(bullet);
				
			} 
		}
	}
	
	for (Enemy* enemy : enemies_)
	{
		aabb2 = enemy->GetAABB();
		for (P_Bullet* bullet : bullets_)
		{
			aabb1 = bullet->GetAABB();
			if (IsCollition(aabb1, aabb2))
			{
				enemy->OnCollition(bullet);
				bullet->OnCollition(enemy);

				score += 150;
			}
		}
	}


#pragma endregion
	

}



void Game::Draw()
{
	
	Sprite::PreDraw();

	timeSprite_->Draw();
	
	pointSprite_->Draw();


	Sprite::PostDraw();

	Model::PreDraw();


		// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_)
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine)
		{
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}


skydome_->Draw();

#pragma region プレイヤー


if (!player_->IsDead())
{
// 自キャラの描画 下記のフェーズのみ描画
	if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn || phase_ == Phase::kEnemyDeath)
	{
		player_->Draw();
	}
}

	cursor_->Draw();

	// パーティクル(プレイヤー)
	if (phase_ == Phase::kDeath)
	{
		if ("deathParticle", true)
		{
			P_Particles_->Draw();
		}
	}

	if (phase_ == Phase::kPlay) 
	{
		
		// 弾の継続時間が0になるまで撃てる
		for (P_Bullet* bullet : bullets_)
		{
			bullet->Draw();
		}
	}

#pragma endregion

#pragma region 敵

	// 敵の描画 下記のフェーズのみ描画
	if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn || phase_ == Phase::kDeath) 
	{
		for (Enemy* enemy : enemies_)
		{
			std::srand(static_cast<unsigned int>(std::time(nullptr)));
		
			enemy->Draw();
		}

	}
	// パーティクル(敵)
	if (phase_ == Phase::kEnemyDeath)
	{
		if ("E_deathParticle", true)
		{
			E_Particles_->Draw();
		}
	}
	

#pragma endregion

	Model::PostDraw();
}





Game::~Game()
{
	delete sprite_;
	
	delete player_;
	delete cursor_;
	for (P_Bullet* bullet : bullets_)
	{
		delete bullet;
	}
	delete P_Particles_;
	for (Enemy* enemy : enemies_)
	{
		delete enemy;
	}
    
	delete E_Particles_;

	delete skydome_;

	// フェード
	delete fade_;

	// 3Dモデルデータの解放
	delete model_;

	// デバッグカメラの解放
	delete debugCamera_;

	delete mapChipField_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();



	delete timeSprite_;
	delete pointSprite_;
}
