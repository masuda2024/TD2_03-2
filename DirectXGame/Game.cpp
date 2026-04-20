#include "Game.h"
#include "MapChipField.h"
#include "MyMath.h"
#include <random>


using namespace KamataEngine;
using namespace MathUtility;

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

#pragma region 天球

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	modelEarth_ = KamataEngine::Model::CreateFromOBJ("earth", true);
	modelMoon_ = KamataEngine::Model::CreateFromOBJ("moon", true);

#pragma endregion

#pragma region 回復アイテム

	modelRecovery_ = KamataEngine::Model::CreateFromOBJ("kaifuku", true);
	recovery_ = new Recovery();
	KamataEngine::Vector3 recoveryPosition = {0, 10.0f, 0};
	recovery_->Initialize(modelRecovery_, &camera_, recoveryPosition);

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

	// フェード
	fade_->Update();
	ImGui::Text("C : Clear  ,  O : Over");
	ImGui::Text("0(Zero) : DebugCamera ");

	CheckAllCollisions();

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
	}
#pragma endregion
}

void Game::Draw()
{
	// スプライト
	Sprite::PreDraw();

#pragma region UI
	_playerHPSprite_->Draw();
	_enemyHPSprite_->Draw();

	playerHPSprite_->Draw();
	enemyHPSprite_->Draw();
#pragma endregion

	Sprite::PostDraw();

	// モデル
	Model::PreDraw();

#pragma region 天球

	modelSkydome_->Draw(worldTransform_, camera_);
	modelEarth_->Draw(worldTransform_, camera_);
	modelMoon_->Draw(worldTransform_, camera_);

#pragma endregion

#pragma region 回復アイテム

	recovery_->Draw();

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
}
