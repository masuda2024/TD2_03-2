#include "Title.h"
#include <cassert>
#include "MyMath.h"
#include "math.h"
#include <cmath>
using namespace KamataEngine;
using namespace MathUtility;



void Title::Initialize()
{
	
	T_FontHandle_ = TextureManager::Load("UI/TitleFont.png");
	T_FontSprite_ = KamataEngine::Sprite::Create(T_FontHandle_, {256, 50});
	
	
	CreditsHandle_ = TextureManager::Load("Scenes/Credits.png");
	CreditsSprite_ = KamataEngine::Sprite::Create(CreditsHandle_, {10, 518});
	


	
	// Springin ボタン・システム(1)　決定2
	Botan_ = Audio::GetInstance()->LoadWave("Sounds/sound/Decision2.mp3");


	// カメラの初期化
	camera_.Initialize();
	// ワールド変換の初期化
	worldTransform_.Initialize();
	




	
	WTS_title_.Initialize();
	WTS_title_.translation_ = {0, 0, 0};
	MS_title_ = Model::CreateFromOBJ("skydome", true);



	
	WTP_title_.Initialize();
	WTP_title_.translation_ = {-17.0f, -4.7f, -20.0f};
	WTP_title_.rotation_.y = 0.5f;
	WTP_title_.scale_ = {1.5f, 1.5f, 1.5f};
	MP_title_ = Model::CreateFromOBJ("H_ziki", true);


	
	WTE_title_.Initialize();
	WTE_title_.translation_ = {17.0f, 4.7f, 10.0f};
	WTE_title_.rotation_.y = -2.7f;
	WTE_title_.scale_ = {1.5f, 1.5f, 1.5f};
	ME_title_ = Model::CreateFromOBJ("kaizyu1", true);


	WT_Cursor_title_.Initialize();
	WT_Cursor_title_.translation_ = {13.5f, 4.7f, -1.0f};
	WT_Cursor_title_.rotation_.y = -1.3f;
	WT_Cursor_title_.scale_ = {1.5f, 1.5f, 1.5f};
	M_Cursor_title_ = Model::CreateFromOBJ("Cursor", true);


	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void Title::Update() 
{



	T_FontSprite_->SetSize({768, 144});


	
	WTS_title_.matWorld_ = MakeAffineMatrix(WTS_title_.scale_, WTS_title_.rotation_, WTS_title_.translation_);
	WTS_title_.TransferMatrix();

	WTP_title_.matWorld_ = MakeAffineMatrix(WTP_title_.scale_, WTP_title_.rotation_, WTP_title_.translation_);
	WTP_title_.TransferMatrix();

	WTE_title_.matWorld_ = MakeAffineMatrix(WTE_title_.scale_, WTE_title_.rotation_, WTE_title_.translation_);
	WTE_title_.TransferMatrix();

	WT_Cursor_title_.matWorld_ = MakeAffineMatrix(WT_Cursor_title_.scale_, WT_Cursor_title_.rotation_, WT_Cursor_title_.translation_);
	WT_Cursor_title_.TransferMatrix();




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
			finishedTitle_ = true;
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
			finishedTitle_ = true;
		}
		break;
	}
}

void Title::Draw() 
{
	// DirectXCommonインスタンスの取得
	//DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	
	Model::PreDraw();

	MS_title_->Draw(WTS_title_, camera_);
	MP_title_->Draw(WTP_title_, camera_);
	ME_title_->Draw(WTE_title_, camera_);
	M_Cursor_title_->Draw(WT_Cursor_title_, camera_);
	Model::PostDraw();


	Sprite::PreDraw();

	T_FontSprite_->Draw();
	CreditsSprite_->Draw();

	Sprite::PostDraw();

	// フェード
	fade_->Draw();
}

Title::~Title() 
{

	delete MS_title_;
	delete MP_title_;
	delete ME_title_;
	delete M_Cursor_title_;
	//  フェード
	delete fade_;
	// タイトルのスプライト
	delete T_FontSprite_;
	//クレジット
	delete CreditsSprite_;
}