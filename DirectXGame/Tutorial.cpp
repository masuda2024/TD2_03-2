#include "Tutorial.h"
using namespace KamataEngine;

void Tutorial::Initialize()
{
	// チュートリアルのスプライト
	t_Handle_ = TextureManager::Load("Scenes/tdTutorial.png");
	
	
	//tutorialSprite_ = KamataEngine::Sprite::Create(t_Handle_, {0,0});

	
	tutorialPos_.x = 0;
	tutorialPos_.y = 0;
	//tutorialPos_ = {0, 0};

	tutorialSprite_ = KamataEngine::Sprite::Create(t_Handle_, tutorialPos_);




	Botan_ = Audio::GetInstance()->LoadWave("Sounds/sound/Decision2.mp3");

	// カメラの初期化
	camera_.Initialize();
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransformPlayer_.Initialize();

	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void Tutorial::Update() 
{

	
	
	tutorialSprite_->SetPosition(tutorialPos_);

	
		if (Input::GetInstance()->TriggerKey(DIK_A) && tutorialPos_.x == 0) 
		{
			scroll_L = true;
		}
		if (scroll_L) 
		{
			tutorialPos_.x -= scrollSpeed_;
		}
		if (tutorialPos_.x <= -1280) 
		{
			scroll_L = false;
		}
	
	

		if (Input::GetInstance()->TriggerKey(DIK_D) && tutorialPos_.x == -1280)
		{
			scroll_R = true;
		}
		if (scroll_R) 
		{
			tutorialPos_.x += scrollSpeed_;
		}
		if (tutorialPos_.x >= 0)
		{
			scroll_R = false;
		}
	

	









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
			finishedTU_ = true;
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
			finishedTU_ = true;
		}
		break;
	}
}

void Tutorial::Draw() 
{
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Sprite::PreDraw(dxCommon->GetCommandList());

	tutorialSprite_->Draw();

	Sprite::PostDraw();

	// フェード
	fade_->Draw();
}

Tutorial::~Tutorial() 
{
	if (fade_) 
	{
		delete fade_;
		fade_ = nullptr;
	}
	delete tutorialSprite_;
	
}