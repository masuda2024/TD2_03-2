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



using namespace KamataEngine;

#pragma region
#pragma endregion

void Game::Initialize()
{
#pragma region

#pragma endregion

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();


	//ブロック
	modelBlock_ = Model::CreateFromOBJ("block");
	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	// マップチップフィールドの初期化
	mapChipField_->LoadMapchipCsv("Resources/blocks.csv");
	GenerateBlocks();
	
	













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
	KamataEngine::Vector3 playerPosition = {0,0, 0};
	player_->Initialize(modelPlayer_, &camera_, playerPosition);
	player_->SetMapChipField(mapChipField_);



	// プレイヤーの弾
	for (int i = 0; i < 28; i++)
	{
		P_Bullet* bullet = new P_Bullet();
		bullet->Initialize(modelPlayerBullet_, &camera_, player_);
		bullet->SetMapChipField(mapChipField_);
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
	//std::srand(static_cast<unsigned int>(std::time(nullptr)));

	//const int mapWidth = 50;
	//const int mapHeight = 20;
	//const int enemyCount = 7;
/*
	std::vector<KamataEngine::Vector2> enemyTilePositions;

	for (int i = 0; i < enemyCount; i++)
	{
		KamataEngine::Vector2 pos;
		pos.x = static_cast<float>(rand() % mapWidth);  // 0 ～ mapWidth-1
		pos.y = static_cast<float>(rand() % mapHeight); // 0 ～ mapHeight-1

		enemyTilePositions.push_back(pos);


	}*/
    
	    // 敵座標をマップチップ番号で指定
	std::vector<KamataEngine::Vector2> enemyTilePositions = 
	{

	    {1, 0},
	    {15, 11},
		{25, 5},
		{35, 8},
		{45, 3},
		{60, 10},
        {75, 6 }
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



	ImGui::Text("Score %d", score);




	
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
		//float enemyHpRatio = (float)enemy->E_GetHP() / (float)enemy->E_GetMaxHP();
		float enemyHpRatio = (float)enemy->E_GetHP() / (float)enemy->E_GetMaxHP();
		enemyHpRatio = std::clamp(enemyHpRatio, 0.0f, 1.0f);
		pointSprite_->SetSize({enemyHpRatio * 1280.0f, 30.0f}); // 幅200px、高さ20px
		pointSprite_->SetPosition({0, 0});                 // 左上少し下に表示





		
	}
	

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

	delete mapChipField_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

}
