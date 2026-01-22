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

using namespace KamataEngine;

#pragma region
#pragma endregion

void Game::Initialize()
{
#pragma region

#pragma endregion

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

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
	KamataEngine::Vector3 playerPosition = {0, 0, 0};
	player_->Initialize(modelPlayer_, &camera_, playerPosition);

	// プレイヤーの弾
	for (int i = 0; i < 28; i++)
	{
		P_Bullet* bullet = new P_Bullet();
		bullet->Initialize(modelPlayerBullet_, &camera_, player_);
		bullets_.push_back(bullet);
	}
	// プレイヤーのデスパーティクル
	P_Particles_ = new P_DeathParticle();
	P_Particles_->Initialize(model_P_Particle_, &camera_, playerPosition);

	pointHandle_ = TextureManager::Load("Point.png");
	pointSprite_ = KamataEngine::Sprite::Create(pointHandle_, {0, 0});


#pragma endregion

#pragma region 敵

	
	// 敵の3Dモデル
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	
	model_E_Particle_ = Model::CreateFromOBJ("E_deathParticle", true);

	// 敵の生成
	//enemy_ = new Enemy();
	//KamataEngine::Vector3 enemyPosition = {35, 5, 0};
	//enemy_->Initialize(modelEnemy_, &camera_, enemyPosition);

	// 敵の座標
	
	    // 敵生成
	
	/**/

	
	// 初期化時に1回だけ
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	const int mapWidth = 50;
	const int mapHeight = 20;
	const int enemyCount = 7;

	std::vector<KamataEngine::Vector2> enemyTilePositions;

	for (int i = 0; i < enemyCount; i++)
	{
		KamataEngine::Vector2 pos;
		pos.x = static_cast<float>(rand() % mapWidth);  // 0 ～ mapWidth-1
		pos.y = static_cast<float>(rand() % mapHeight); // 0 ～ mapHeight-1

		enemyTilePositions.push_back(pos);
	}
    /*
	    // 敵座標をマップチップ番号で指定
	std::vector<KamataEngine::Vector2> enemyTilePositions = 
	{
	   
	    {3,  5},
	    {5,  10},
	    {5, 5},
        {2,  1},
        {0,  7} 

		
	};*/







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

void Game::Update()
{
	// フェード
	fade_->Update();




	




	
	// 天球の更新
	skydome_->Update();
#pragma region プレイヤー

	player_->Update();
	// プレイヤーの攻撃を呼び出す
	
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) 
	{
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
	}
#pragma endregion

#pragma region 敵

	for (Enemy* enemy : enemies_)
	{
		enemy->Update();
		// 敵HP
		float enemyHpRatio = (float)enemy->E_GetHP() / (float)enemy->E_GetMaxHP();
		enemyHpRatio = std::clamp(enemyHpRatio, 0.0f, 1.0f);
		pointSprite_->SetSize({enemyHpRatio * 1280.0f, 30.0f}); // 幅200px、高さ20px
		pointSprite_->SetPosition({0, 0});                 // 左上少し下に表示
		

	}
	

#pragma endregion

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






				// 初期化時に1回だけ
				std::srand(static_cast<unsigned int>(std::time(nullptr)));




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

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) 
	{
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif // _DEBUG

	if (isDebugCameraActive_)
	{
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else
	{
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
			}
		}
	}


#pragma endregion

}


void Game::Draw()
{
	//DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//Model::PreDraw(dxCommon->GetCommandList());

	



	Sprite::PreDraw();

	pointSprite_->Draw();

	Sprite::PostDraw();

	Model::PreDraw();

skydome_->Draw();

#pragma region プレイヤー

	// 自キャラの描画 下記のフェーズのみ描画
	if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn || phase_ == Phase::kEnemyDeath)
	{
		player_->Draw();
	}
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
}
