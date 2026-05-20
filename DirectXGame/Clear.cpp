#include "Clear.h"
#include <cassert>
#include "MyMath.h"
#include "math.h"

using namespace KamataEngine;


void Clear::Initialize() 
{
	
	ClearFontHandle_ = TextureManager::Load("UI/GAME_CLEAR.png");
	ClearFontSprite_ = Sprite::Create(ClearFontHandle_, {283, 20});
	




	// Springin ボタン・システム(1)　決定2
	Botan_ = Audio::GetInstance()->LoadWave("Sounds/sound/Decision2.mp3");




	

	// 効果音ラボ 戦闘[2] 爆発4
	Explosion_Handle_ = Audio::GetInstance()->LoadWave("Sounds/sound/Explosion4.mp3");








	// カメラの初期化
	camera_.Initialize();
	// ワールド変換の初期化
	worldTransform_.Initialize();
	


	worldTransformSky_.Initialize();
	worldTransformSky_.translation_ = {0, 0, 0};
	modelSky_ = Model::CreateFromOBJ("Sky_Sphere", true);

	worldTransformGround_.Initialize();
	worldTransformGround_.translation_ = {0, -1, -45};
	modelGround_ = Model::CreateFromOBJ("Ground", true);

	worldTransformEnemy2_.Initialize();
	worldTransformEnemy2_.translation_ = {0.0f, -1.3f, -45.0f};
	worldTransformEnemy2_.rotation_.x = 0.7f;
	worldTransformEnemy2_.rotation_.y = 3.0f;
	worldTransformEnemy2_.rotation_.z = 1.0f;
	worldTransformEnemy2_.scale_ = {0.5f, 0.5f, 0.5f};
	modelEnemy2_ = Model::CreateFromOBJ("kaizyu1", true);







	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void Clear::Update()
{

	//爆発音
	if (isExplosionPlayed_ == 0)
	{
		Explosion_Sound_ = Audio::GetInstance()->PlayWave(Explosion_Handle_, false);
		isExplosionPlayed_ = true;
	}








	ClearFontSprite_->SetSize({704, 352});

	



	worldTransformSky_.matWorld_ = MakeAffineMatrix(worldTransformSky_.scale_, worldTransformSky_.rotation_, worldTransformSky_.translation_);
	worldTransformSky_.TransferMatrix();

	worldTransformGround_.matWorld_ = MakeAffineMatrix(worldTransformGround_.scale_, worldTransformGround_.rotation_, worldTransformGround_.translation_);
	worldTransformGround_.TransferMatrix();

	worldTransformEnemy2_.matWorld_ = MakeAffineMatrix(worldTransformEnemy2_.scale_, worldTransformEnemy2_.rotation_, worldTransformEnemy2_.translation_);
	worldTransformEnemy2_.TransferMatrix();



	switch (phase_)
	{
	case Phase::kMain:

		// タイトルシーンの終了条件
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->IsTriggerMouse(0))
		{
			Audio::GetInstance()->PlayWave(Botan_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedC_ = true;
		}

		break;
	case Phase::kFadeIn:
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kFadeOut:
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			finishedC_ = true;
		}
		break;
	}
}

void Clear::Draw()
{
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	
	Model::PreDraw();

	
	modelSky_->Draw(worldTransformSky_, camera_);
	modelGround_->Draw(worldTransformGround_, camera_);
	modelEnemy2_->Draw(worldTransformEnemy2_, camera_);

	Model::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());

	

	ClearFontSprite_->Draw();

	Sprite::PostDraw();

	// フェード
	fade_->Draw();
}

Clear::~Clear() 
{

	delete modelSky_;
	delete modelGround_;
	delete modelEnemy2_;


	//  フェード
	delete fade_;
	delete ClearFontSprite_;
}