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
	bool IsFinishedC() const { return finishedC_; }
	
	
	//リトライ
	bool finishedC_2 = false;
	bool IsFinishedC2() const { return finishedC_2; }






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


	uint32_t C_TextHandle_ = 0;
	KamataEngine::Sprite* C_TextSprite_ = nullptr;


	uint32_t UI_R_Handle_ = 0;
	KamataEngine::Sprite* UI_RSprite_ = nullptr;
	uint32_t UI_R_Handle_2 = 0;
	KamataEngine::Sprite* UI_RSprite_2 = nullptr;


	uint32_t UI_T_Handle_ = 0;
	KamataEngine::Sprite* UI_TSprite_ = nullptr;
	uint32_t UI_T_Handle_2 = 0;
	KamataEngine::Sprite* UI_TSprite_2 = nullptr;








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
