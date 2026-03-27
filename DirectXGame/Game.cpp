#include "Game.h"
#include "MapChipField.h"
#include "MyMath.h"

#include <cstdlib>
#include <ctime>
#include <random>

#include "Windows.h"
#include "imgui.h"
#include "math.h"

#include "CameraController.h"
#include "Fade.h"

#include "P_Bullet.h"
#include "Player.h"

#include "Enemy.h"

using namespace KamataEngine;
using namespace MathUtility;

#pragma region
#pragma endregion

void Game::Initialize()
{
#pragma region

#pragma endregion
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
#pragma region マップ関係
	// ブロック
	modelBlock_ = Model::CreateFromOBJ("block");
	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	// マップチップフィールドの初期化
	mapChipField_->LoadMapchipCsv("Resources/blocks.csv");
	GenerateBlocks();

#pragma endregion

#pragma region スカイドーム

	modelskydome_ = Model::CreateFromOBJ("SkyDome", true);
	// スカイドームの生成
	skydome_ = new Skydome();
	skydome_->Initialize(modelskydome_, textureHandle_, &camera_);

#pragma endregion

#pragma region プレイヤー

	// プレイヤー
	// modelPlayer_ = Model::CreateFromOBJ("player", true);

	modelPlayer_ = Model::CreateFromOBJ("H_ziki", true);

	// プレイヤーの弾
	// modelPlayerBullet_ = Model::CreateFromOBJ("bullet", true);
	modelPlayerBullet_ = Model::CreateFromOBJ("Z_bullet", true);

	// プレイヤーの弾の発射音声
	P_Shot_ = Audio::GetInstance()->LoadWave("Sounds/sound/Shot.mp3");

	// パーティクルの3Dモデルデータの生成
	model_P_Particle_ = Model::CreateFromOBJ("deathParticle", true);

	// プレイヤーの生成
	player_ = new Player();

	// プレイヤーの座標を指定
	// Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(6, 30);
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

	// 得点
	pointHandle_ = TextureManager::Load("Point.png");
	pointSprite_ = KamataEngine::Sprite::Create(pointHandle_, {0, 0});

	// 時間
	timeHandle_ = TextureManager::Load("Time.png");
	timeSprite_ = KamataEngine::Sprite::Create(timeHandle_, {0, 0});

#pragma endregion

#pragma region カーソル

	TextureManager::Load("Cursor.png");
	modelCursor_ = Model::CreateFromOBJ("Cursor");

	// カーソル
	cursor_ = new Cursor();

	// カーソルの初期化
	KamataEngine::Vector3 cursorPosition = {15, 0, 0};
	// cursor_->Initialize(modelCursor_, &camera_, playerPosition);
	cursor_->Initialize(modelCursor_, &camera_, cursorPosition);

	cursor_->SetMapChipField(mapChipField_);

#pragma endregion

#pragma region 敵

	// 敵の3Dモデル
	// modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	modelEnemy_ = Model::CreateFromOBJ("kaizyu1", true);

	model_E_Particle_ = Model::CreateFromOBJ("E_deathParticle", true);

	// 敵のHP
	enemyHPHandle_ = TextureManager::Load("Ehp.png");
	enemyHPSprite_ = KamataEngine::Sprite::Create(enemyHPHandle_, {1050, 0});

	// 敵の生成
	enemy_ = new Enemy();
	// 敵の座標
	KamataEngine::Vector3 enemyPosition = {40, 5, 0};
	enemy_->Initialize(modelEnemy_, &camera_, enemyPosition);
	// enemy_->SetMapChipField(mapChipField_);

	// 敵の弾
	modelE_Bullet_ = Model::CreateFromOBJ("E_bullet", true);
	// 敵の弾
	E_Bullet_ = new E_Bullet();
	E_Bullet_->Initialize(modelE_Bullet_, &camera_, enemyPosition, E_B_velocity_);

	
	EnemyAttack();
	

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
	ImGui::Text("Enemy HP : %d", enemy_->E_GetHP());

	// フェード
	fade_->Update();

#pragma region UI

	// enemyHPのスプライト
	enemyHPHandle_ = TextureManager::Load("Sprites/Ehp.png");
	enemyHPSprite_ = KamataEngine::Sprite::Create(enemyHPHandle_, {1050, 0});

	_enemyHPHandle_ = TextureManager::Load("Sprites/Ehp_.png");
	_enemyHPSprite_ = KamataEngine::Sprite::Create(_enemyHPHandle_, {1050, 0});

	// 敵HP
	float enemyHpRatio = (float)enemy_->E_GetHP() / (float)enemy_->E_GetMaxHP();
	enemyHpRatio = std::clamp(enemyHpRatio, 0.0f, 1.0f);
	enemyHPSprite_->SetSize({enemyHpRatio * 300.0f, 30.0f}); // 幅200px、高さ20px
	enemyHPSprite_->SetPosition({980, 0});                   // 左上少し下に表示

	_enemyHPSprite_->SetSize({300.0f, 30.0f}); // 幅200px、高さ20px
	_enemyHPSprite_->SetPosition({980, 0});    // 左上少し下に表示

	playerHPHandle_ = TextureManager::Load("Sprites/hp.png");
	playerHPSprite_ = KamataEngine::Sprite::Create(playerHPHandle_, {0, 0});
	_playerHPHandle_ = TextureManager::Load("Sprites/hp_.png");
	_playerHPSprite_ = KamataEngine::Sprite::Create(_playerHPHandle_, {0, 0});

	// プレイヤーHP
	float hpRatio = (float)player_->GetHP() / (float)player_->GetMaxHP();
	hpRatio = std::clamp(hpRatio, 0.0f, 1.0f);
	playerHPSprite_->SetSize({hpRatio * 300.0f, 30.0f}); // 例：幅200px、高さ20px
	playerHPSprite_->SetPosition({0, 0});

	_playerHPSprite_->SetSize({300.0f, 30.0f}); // 例：幅200px、高さ20px
	_playerHPSprite_->SetPosition({0, 0});

	/*
	time -= 20;

	float timeRatio = (float)time / (float)maxtime;
	timeRatio = std::clamp(timeRatio, 0.0f, 1.0f);
	timeSprite_->SetSize({timeRatio * 1280.0f, 30.0f}); // 幅200px、高さ20px
	timeSprite_->SetPosition({0, 0});


	float scoreRatio = (float)score / (float)MaxScore;
	scoreRatio = std::clamp(scoreRatio, 0.0f, 1.0f);
	pointSprite_->SetSize({scoreRatio * 200.0f, 20.0f}); // 幅200px、高さ20px
	pointSprite_->SetPosition({1060, 10});
*/

#pragma endregion

#pragma region 天球
	// 天球の更新
	skydome_->Update();
#pragma endregion

#pragma region プレイヤー

	player_->Update();
	player_->RotateX();
	player_->RotateZ();
	// プレイヤーの攻撃を呼び出す

	if (Input::GetInstance()->IsTriggerMouse(0)) 
	{
		Audio::GetInstance()->PlayWave(P_Shot_);
		for (P_Bullet* bullet : bullets_)
		{
			if (!bullet->IsActive())
			{

				bullet->StartAttack_at_Mouse();
				break;
			}
		}
	}

	/*
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
	    Audio::GetInstance()->PlayWave(P_Shot_);
	    for (P_Bullet* bullet : bullets_)
	    {
	        if (!bullet->IsActive())
	        {

	            bullet->StartAttack_at_Mouse();

	            break;
	        }
	    }
	}
*/

	// プレイヤーの弾を更新
	for (P_Bullet* bullet : bullets_)
	{
		bullet->Update();
		if (!bullet->GetReflection() && bullet->IsActive()) 
		{
			score += 200;
		}
		// ImGui::Text("Score x 2 %d", bullet->GetReflection());
	}

#pragma endregion

#pragma region カーソル

	cursor_->Update();
	Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();
	ImGui::Text("Mouse Move X: %ld, Y: %ld, Z: %ld", mouseMove.lX, mouseMove.lY, mouseMove.lZ);

#pragma endregion

#pragma region 敵


	enemy_->Update();
	// 発射タイマーをカウントダウンする
	fireTimer--;

	if (fireTimer == 0) 
	{
		EnemyAttack();
		fireTimer = kFireInterval;
	}

	// 敵の弾を更新
	for (E_Bullet* Ebullet : E_bullets_) 
	{
		Ebullet->Update();
	}

	// ImGui::Text("Score %d", score);

#pragma endregion

#pragma region デバッグカメラ

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

		if (enemy_->IsEnemyDead() == true) 
		{
			std::vector<KamataEngine::Vector2> enemyTilePositions;

			// 敵の座標を取得
			const KamataEngine::Vector3 E_deathParticlesPosition = enemy_->GetWorldPosition();

			// パーティクル
			E_Particles_ = new E_DeathParticle();
			E_Particles_->Initialize(model_E_Particle_, &camera_, E_deathParticlesPosition);
		}

		if (enemy_->IsEnemyDead() == true) 
		{
			phase_ = Phase::kEnemyDeath;
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
	} else {
		camera_.TransferMatrix();
		camera_.UpdateMatrix();
	}

#pragma endregion
}

// 敵の攻撃

void Game::EnemyAttack()
{
	fireTimer = kFireInterval;

	// 弾の速度
	const float kEBulletSpeed = 1.0f;
	Vector3 E_bulletVelocity = {kEBulletSpeed, 0.0f, 0.0f};

	// 座標を取得(弾を自キャラと同じ位置にする)
	const KamataEngine::Vector3 enemyBulletPosition = enemy_->GetWorldPosition();

	E_Bullet_ = new E_Bullet();
	E_Bullet_->Initialize(modelE_Bullet_, &camera_, enemyBulletPosition, E_bulletVelocity);

	E_bullets_.push_back(E_Bullet_);
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

		if (enemy_->IsEnemyDead() == true)
		{
			// デス演出フェーズに切り替え
			phase_ = Phase::kEnemyDeath;
		}
#pragma endregion

		break;

	case Phase::kDeath:
		// デス演出フェーズの処理

		if (P_Particles_)
		{
			// シーン終了
			// ゲームオーバーへ
			finishedGAME_ = true;
		}

		break;
	case Phase::kEnemyDeath:

		// シーン終了
		// ゲームクリアへ
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

		aabb2 = enemy_->GetAABB();
		if (IsCollition(aabb1, aabb2))
		{
			// 自キャラの衝突時関数を呼び出す
			bullet->OnCollition(enemy_);
			enemy_->OnCollition(bullet);
		}
	}

#pragma endregion

#pragma region 敵の弾とプレイヤー
	AABB2 aabb3, aabb4;

	for (E_Bullet* Ebullet : E_bullets_)
	{
		aabb3 = Ebullet->GetAABB2();

		aabb4 = player_->GetAABB2();
		if (IsCollition2(aabb3, aabb4))
		{
			Ebullet->OnCollition2(player_);
			player_->OnCollition2(Ebullet);
		}
	}
#pragma endregion
}

void Game::Draw()
{

	Sprite::PreDraw();

	// timeSprite_->Draw();

	// pointSprite_->Draw();

	_playerHPSprite_->Draw();
	_enemyHPSprite_->Draw();

	playerHPSprite_->Draw();
	enemyHPSprite_->Draw();

	Sprite::PostDraw();

	Model::PreDraw();

#pragma region カーソル
	cursor_->Draw();
#pragma endregion

#pragma region ブロック
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
#pragma endregion

#pragma region 天球
	skydome_->Draw();
#pragma endregion

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

		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		enemy_->Draw();

		for (E_Bullet* Ebullet : E_bullets_) 
		{
			Ebullet->Draw();
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
	// 3Dモデルデータの解放
	delete model_;
	// スプライトの解放
	delete sprite_;
#pragma region プレイヤー
	delete player_;

	for (P_Bullet* bullet : bullets_)
	{
		delete bullet;
	}
	delete P_Particles_;

#pragma endregion

#pragma region 敵

	delete enemy_;
	delete E_Particles_;

	for (E_Bullet* Ebullet : E_bullets_) 
	{
		delete Ebullet;
	}

#pragma endregion

#pragma region UI
	delete cursor_;

	delete playerHPSprite_;
	delete _playerHPSprite_;

	delete enemyHPSprite_;
	delete _enemyHPSprite_;

	delete timeSprite_;
	delete pointSprite_;
#pragma endregion

#pragma region 天球
	delete skydome_;
#pragma endregion

#pragma region マップ関係

	// マップチップを解放
	delete mapChipField_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_)
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
#pragma endregion

#pragma region フェード・デバッグカメラ

	// フェード
	delete fade_;

	// デバッグカメラの解放
	delete debugCamera_;

#pragma endregion
}