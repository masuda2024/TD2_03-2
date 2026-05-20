#include "Over.h"
#include <cassert>
#include "MyMath.h"
#include "math.h"

using namespace KamataEngine;

void Over::Initialize() 
{
	OverFontHandle_ = TextureManager::Load("UI/GAME_OVER.png");
	OverFontSprite_ = Sprite::Create(OverFontHandle_, {283, 20});
	




	// Springin ボタン・システム(1)　決定2
	Botan_ = Audio::GetInstance()->LoadWave("Sounds/sound/Decision2.mp3");


	

	// 効果音ラボ 機械・乗り物[1] 戦闘機上空通過1
	Plane_Handle_ = Audio::GetInstance()->LoadWave("Sounds/sound/FighterjetsFlyOverhead1.mp3");




	// カメラの初期化
	camera_.Initialize();
	// ワールド変換の初期化
	worldTransform_.Initialize();
	//worldTransformPlayer_.Initialize();



	worldTransformSky_.Initialize();
	worldTransformSky_.translation_ = {0, 0, 0};
	modelSky_ = Model::CreateFromOBJ("Sky_Sphere", true);


	worldTransformRunway_.Initialize();
	worldTransformRunway_.translation_ = {0, -2, -40};
	modelRunway_ = Model::CreateFromOBJ("Runway", true);

	worldTransformPlayer2_.Initialize();
	worldTransformPlayer2_.translation_ = {0.0f, -1.7f, -40.0f};
	worldTransformPlayer2_.rotation_.y = 2.3f;
	worldTransformPlayer2_.scale_ = {0.5f, 0.5f, 0.5f};
	modelPlayer2_ = Model::CreateFromOBJ("H_ziki", true);


	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void Over::Update() 
{

	//飛行機音
	if (isPlanePlayed_ == 0) 
	{
		Plane_Sound_ = Audio::GetInstance()->PlayWave(Plane_Handle_, false);
		isPlanePlayed_ = true;
	}



	OverFontSprite_->SetSize({704, 352});

	


	worldTransformSky_.matWorld_ = MakeAffineMatrix(worldTransformSky_.scale_, worldTransformSky_.rotation_, worldTransformSky_.translation_);
	worldTransformSky_.TransferMatrix();

	worldTransformRunway_.matWorld_ = MakeAffineMatrix(worldTransformRunway_.scale_, worldTransformRunway_.rotation_, worldTransformRunway_.translation_);
	worldTransformRunway_.TransferMatrix();

	worldTransformPlayer2_.matWorld_ = MakeAffineMatrix(worldTransformPlayer2_.scale_, worldTransformPlayer2_.rotation_, worldTransformPlayer2_.translation_);
	worldTransformPlayer2_.TransferMatrix();




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
			finishedO_ = true;
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
			finishedO_ = true;
		}
		break;
	}





}

void Over::Draw()
{
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	
	Model::PreDraw();

	modelSky_->Draw(worldTransformSky_, camera_);
	modelRunway_->Draw(worldTransformRunway_, camera_);
	modelPlayer2_->Draw(worldTransformPlayer2_, camera_);
	
	Model::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());

	OverFontSprite_->Draw();

	Sprite::PostDraw();




	// フェード
	fade_->Draw();
}

Over::~Over()
{

	//  フェード
	delete fade_;
	

	delete modelSky_;
	delete modelRunway_;
	delete modelPlayer2_;
	
	delete OverFontSprite_;
}