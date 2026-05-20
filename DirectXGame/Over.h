#pragma once
#include "Fade.h"
#include "KamataEngine.h"
class Over
{
public:
	enum class Phase
	{
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 終了フラグ
	bool finishedO_ = false;
	// デスフラグのgetter
	bool IsFinishedO() const { return finishedO_; }

	void Initialize();
	void Update();
	void Draw();

	// デストラクタ
	~Over();
	// void ChangeScene();
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

private:
	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;
	
	
	// カメラ
	KamataEngine::Camera camera_;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	
	#pragma region 

	KamataEngine::Model* modelSky_ = nullptr;
	KamataEngine::WorldTransform worldTransformSky_;

	KamataEngine::Model* modelRunway_ = nullptr;
	KamataEngine::WorldTransform worldTransformRunway_;


	KamataEngine::Model* modelPlayer2_ = nullptr;
	KamataEngine::WorldTransform worldTransformPlayer2_;



	#pragma endregion

	
	uint32_t OverFontHandle_ = 0;
	KamataEngine::Sprite* OverFontSprite_ = nullptr;




	// フェード
	Fade* fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	uint32_t Botan_ = 0;


	// 飛行機音
	bool isPlanePlayed_ = false;
	uint32_t Plane_Handle_ = 0;
	uint32_t Plane_Sound_ = 0;

};