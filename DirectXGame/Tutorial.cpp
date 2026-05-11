#include "Tutorial.h"
using namespace KamataEngine;

void Tutorial::Initialize()
{


#pragma region 1ページ
	// チュートリアルのスプライト
	t_Handle_ = TextureManager::Load("Scenes/tdTutorial.png");
	
	
	//tutorialSprite_ = KamataEngine::Sprite::Create(t_Handle_, {0,0});

	tutorialPos_.x = 0;
	tutorialPos_.y = 0;
	//tutorialPos_ = {0, 0};

	tutorialSprite_ = KamataEngine::Sprite::Create(t_Handle_, tutorialPos_);

#pragma endregion


#pragma region 2ページ
	// チュートリアルのスプライト
	t_Handle2_ = TextureManager::Load("Scenes/tdTutorial2.png");

	//tutorialPos_2.x = 1280;
	tutorialPos_2.x = 0;
	tutorialPos_2.y = 0;
	
	tutorialSprite2_ = KamataEngine::Sprite::Create(t_Handle2_, tutorialPos_2);

#pragma endregion

#pragma region 3ページ
	// チュートリアルのスプライト
	t_Handle3_ = TextureManager::Load("Scenes/tdTutorial3.png");

	
	//tutorialPos_3.x = 2560;
	tutorialPos_3.x = 0;
	tutorialPos_3.y = 0;
	
	tutorialSprite3_ = KamataEngine::Sprite::Create(t_Handle3_, tutorialPos_3);

#pragma endregion






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
	tutorialSprite2_->SetPosition(tutorialPos_2);
	tutorialSprite3_->SetPosition(tutorialPos_3);


	switch (page_)
	{ 
	case Pages::p1:
		
		/*
		//2ページへ
		if (Input::GetInstance()->TriggerKey(DIK_D) && tutorialPos_.x == 0 && tutorialPos_2.x == 1280 && tutorialPos_3.x == 2560) 
		{
			scroll_L = true;
		}
		if (scroll_L) 
		{
			tutorialPos_.x -= scrollSpeed_;
			tutorialPos_2.x -= scrollSpeed_;
			tutorialPos_3.x -= scrollSpeed_;
		}
		if (tutorialPos_.x <= -1280 && tutorialPos_2.x <= 0 && tutorialPos_3.x <= 1280) 
		{
			scroll_L = false;
			page_ = Pages::p2;
		}
*/


		// 2ページへ
		if (Input::GetInstance()->TriggerKey(DIK_D))
		{
			page_ = Pages::p2;
		}

		break;

	case Pages::p2:


		// 1ページへ
		if (Input::GetInstance()->TriggerKey(DIK_A)) 
		{
			page_ = Pages::p1;
		}
		// 3ページへ
		if (Input::GetInstance()->TriggerKey(DIK_D))
		{
			page_ = Pages::p3;
		}

		/*
		// 1ページへ
		if (Input::GetInstance()->TriggerKey(DIK_A) && tutorialPos_.x == -1280 && tutorialPos_2.x == 0 && tutorialPos_3.x == 1280)
		{
			scroll_R = true;
		}
		if (scroll_R)
		{
			tutorialPos_.x += scrollSpeed_;
			tutorialPos_2.x += scrollSpeed_;
			tutorialPos_3.x += scrollSpeed_;
		}
		if (tutorialPos_.x >= 0 && tutorialPos_2.x >= 1280 && tutorialPos_3.x >= 2560)
		{
			scroll_R = false;
			page_ = Pages::p1;
		}

		// 3ページへ
		if (Input::GetInstance()->TriggerKey(DIK_D) && tutorialPos_.x == -1280 && tutorialPos_2.x == 0 && tutorialPos_3.x == 1280)
		{
			scroll_L = true;
		}
		if (scroll_L) 
		{
			tutorialPos_.x -= scrollSpeed_;
			tutorialPos_2.x -= scrollSpeed_;
			tutorialPos_3.x -= scrollSpeed_;
		}
		if (tutorialPos_.x <= -2560 && tutorialPos_2.x <= -1280 && tutorialPos_3.x <= 0)
		{
			scroll_L = false;
			page_ = Pages::p3;
		}



*/






		break;

	case Pages::p3:

		// 2ページへ
		if (Input::GetInstance()->TriggerKey(DIK_A))
		{
			page_ = Pages::p2;
		}

		/*
		// 2ページへ
		if (Input::GetInstance()->TriggerKey(DIK_A) && tutorialPos_.x <= -2560 && tutorialPos_2.x <= -1280 && tutorialPos_3.x <= 0) 
		{
			scroll_R = true;
		}
		if (scroll_R)
		{
			tutorialPos_.x += scrollSpeed_;
			tutorialPos_2.x += scrollSpeed_;
			tutorialPos_3.x += scrollSpeed_;
		}
		if (tutorialPos_.x == -1280 && tutorialPos_2.x == 0 && tutorialPos_3.x == 1280)
		{
			scroll_R = false;
			page_ = Pages::p2;
		}


*/



		// シーンの終了条件
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->IsTriggerMouse(0)) {
			Audio::GetInstance()->PlayWave(Botan_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedTU_ = true;
		}

		break;
	}



	
		
	

	









	switch (phase_) 
	{
	case Phase::kMain:
		
		// シーンの終了条件(チュートリアルをスキップ)
		if (Input::GetInstance()->TriggerKey(DIK_S))
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

	switch (page_)
	{
	case Pages::p1:
		tutorialSprite_->Draw();
		break;
	case Pages::p2:
		tutorialSprite2_->Draw();
		break;
	case Pages::p3:
		tutorialSprite3_->Draw();
		break;
	}
	
	
	

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
	delete tutorialSprite2_;
	delete tutorialSprite3_;
}