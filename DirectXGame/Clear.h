#pragma once
#include"KamataEngine.h"
#include"Fade.h"
class Clear
{
public:
	enum class Phase 
	{
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 終了フラグ
	bool finishedC_ = false;
	// デスフラグのgetter
	bool IsFinishedC() const { return finishedC_; }

	void Initialize();
	void Update();
	void Draw();

	// デストラクタ
	~Clear();
	// void ChangeScene();
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	
	// マップチップフィールド
	//MapChipField* mapChipField_;

private:
	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;
	// カメラ
	KamataEngine::Camera camera_;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	



	KamataEngine::Model* modelSky_ = nullptr;
	KamataEngine::WorldTransform worldTransformSky_;

	KamataEngine::Model* modelGround_ = nullptr;
	KamataEngine::WorldTransform worldTransformGround_;


	KamataEngine::Model* modelEnemy2_ = nullptr;
	KamataEngine::WorldTransform worldTransformEnemy2_;


	uint32_t ClearFontHandle_ = 0;
	KamataEngine::Sprite* ClearFontSprite_ = nullptr;





	// フェード
	Fade* fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	uint32_t Botan_ = 0;


	// 爆発音
	bool isExplosionPlayed_ = false;
	uint32_t Explosion_Handle_ = 0;
	uint32_t Explosion_Sound_ = 0;
};
