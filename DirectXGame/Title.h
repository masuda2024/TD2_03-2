#pragma once
#include "KamataEngine.h"
#include "Fade.h"

class Title 
{
public:
	enum class Phase 
	{
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 終了フラグ
	bool finishedTitle_ = false;
	bool IsFinishedT() const { return finishedTitle_; }

	void Initialize();
	void Update();
	void Draw();

	// デストラクタ
	~Title();
	// void ChangeScene();
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	
	
private:
	KamataEngine::Model* model_;
	// カメラ
	KamataEngine::Camera camera_;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	


	KamataEngine::Model* MS_title_ = nullptr;
	KamataEngine::WorldTransform WTS_title_;



	KamataEngine::Model* MP_title_ = nullptr;
	KamataEngine::WorldTransform WTP_title_;

	
	KamataEngine::Model* ME_title_ = nullptr;
	KamataEngine::WorldTransform WTE_title_;


	KamataEngine::Model* M_Cursor_title_ = nullptr;
	KamataEngine::WorldTransform WT_Cursor_title_;


	uint32_t T_FontHandle_ = 0;
	KamataEngine::Sprite* T_FontSprite_ = nullptr;




	uint32_t T_StartHandle_ = 0;
	KamataEngine::Sprite* T_StartSprite_ = nullptr;
	uint32_t T_StartHandle_2 = 0;
	KamataEngine::Sprite* T_StartSprite_2 = nullptr;




	uint32_t CreditsHandle_ = 0;
	KamataEngine::Sprite* CreditsSprite_ = nullptr;


	// フェード
	Fade* fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	uint32_t Botan_ = 0;
};
