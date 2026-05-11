#pragma once
#include"KamataEngine.h"
#include"Fade.h"
class Tutorial 
{
public:
	enum class Phase 
	{
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 終了フラグ
	bool finishedTU_ = false;
	// デスフラグのgetter
	bool IsFinishedTU() const { return finishedTU_; }

	void Initialize();
	void Update();
	void Draw();

	// デストラクタ
	~Tutorial();
	// void ChangeScene();
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	
	// マップチップフィールド
	//MapChipField* mapChipField_;




	KamataEngine::Vector2 tutorialPos_ = {0, 0};
	KamataEngine::Vector2 tutorialPos_2 = {0, 0};
	KamataEngine::Vector2 tutorialPos_3 = {0, 0};
	
	
	
	
	//KamataEngine::Vector2 tutorialPos_2 = {1280, 0};
	//KamataEngine::Vector2 tutorialPos_3 = {2560, 0};



	//float scrollSpeed_ = 20.0f;

	//bool scroll_L = false;

	//bool scroll_R = false;



	enum class Pages 
	{
		p1,
		p2,
		p3,
	};
	Pages page_ = Pages ::p1;



private:
	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;
	// モデルプレイヤー
	// KamataEngine::Model* modelPlayer_ = nullptr;
	// カメラ
	KamataEngine::Camera camera_;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;

	// フェード
	Fade* fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	//1ページ
	uint32_t t_Handle_ = 0;
	KamataEngine::Sprite* tutorialSprite_ = nullptr;
	
	// 2ページ
	uint32_t t_Handle2_ = 0;
	KamataEngine::Sprite* tutorialSprite2_ = nullptr;
	
	// 3ページ
	uint32_t t_Handle3_ = 0;
	KamataEngine::Sprite* tutorialSprite3_ = nullptr;


	uint32_t Botan_ = 0;
};
