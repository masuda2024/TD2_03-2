#pragma once
#include "KamataEngine.h"


#include "MapChipField.h"


#include "CameraController.h"
#include "Skydome.h"


#include "Player.h"
#include "Cursor.h"
#include "P_DeathParticle.h"
#include "P_Bullet.h"


#include "Enemy.h"
#include "E_DeathParticle.h"
#include "E_Bullet.h"





#include "Fade.h"
#include <list>
#include <vector>
#define NOMINMAX
#include <Windows.h>
#include <algorithm>
// ゲームシーン

using namespace KamataEngine;
using namespace MathUtility;

/*
struct MouseMove
{
	LONG lx;
	LONG ly;
	LONG lz;
};*/


/*

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) 
{
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) 
		{
			result.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] + m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
		}
	}
	return result;
}
// ビュープロジェクション
Matrix4x4 viewProjectionMatrix;
//ビュープロジェクションの初期化
Matrix4x4 InitializeViewProjectionMatrix(const Camera& camera)
{
	Matrix4x4 view;
	Matrix4x4 projection;
	return Multiply(view, projection);
}
*/





class Game
{
public:
	// デストラクタ
	~Game();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();



#pragma region 終了フラグ

	// 終了フラグ
	bool finishedGAME_ = false;
	// デス(オーバー)フラグのgetter
	bool IsFinishedGAME() const { return finishedGAME_; } // ゲームオーバー

	// 敵を倒した場合の(クリア)フラグ
	bool finishedGAME2_ = false;
	bool IsFinishedGAME2() const { return finishedGAME2_; } ////ゲームクリア

#pragma endregion




	static int GetMouscePosition(int* positionX, int* positionY);
	
	

	



private:



	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	
	// デバックカメラの生成
	// debugCamera_ = new DebugCamera();

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera camera_;

	// スプライト
	KamataEngine::Sprite* sprite_ = nullptr;






#pragma region マップ関係

	// ブロック
	KamataEngine::Model* modelBlock_;
	// マップチップフィールド
	MapChipField* mapChipField_;

	void GenerateBlocks();
#pragma endregion

#pragma region UI

	// 得点テクスチャハンドル
	uint32_t pointHandle_ = 0;
	// スプライト
	KamataEngine::Sprite* pointSprite_ = nullptr;
	uint32_t timeHandle_ = 0;
	KamataEngine::Sprite* timeSprite_ = nullptr;
	// スコア
	int MaxScore = 1000000;
	int score = 0;

#pragma endregion
#pragma region 天球
	// 天球
	Skydome* skydome_ = nullptr;
	KamataEngine::Model* modelskydome_ = nullptr;

#pragma endregion

	
#pragma region プレイヤー

	// プレイヤー
	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	Cursor* cursor_ = nullptr;
	KamataEngine::Model* modelCursor_ = nullptr;


#pragma region プレイヤーの弾

	uint32_t P_Shot_ = 0;

	// 自キャラの弾
	KamataEngine::Model* modelPlayerBullet_ = nullptr;
	// 弾
	std::list<P_Bullet*> bullets_;
	// 速度
	KamataEngine::Vector3 velocity_;
	// 弾の寿命(フレーム数)
	//int playerBulletLifeTime = 20; // 変更可能な左辺値にするためconstを外し型をintに変更    // スペースキーを押して弾を撃つ

#pragma endregion

	// プレイヤーの弾の生成
	P_Bullet* playerBullet_ = nullptr;
	

	// プレイヤーのパーティクル
	P_DeathParticle* P_Particles_ = nullptr;
	KamataEngine::Model* model_P_Particle_ = nullptr;

	

	

#pragma endregion

#pragma region 敵

	// 敵
	Enemy* enemy_ = nullptr;
	//std::list<Enemy*> enemies_;


	KamataEngine::Model* modelEnemy_ = nullptr;

	// 敵のパーティクル
	E_DeathParticle* E_Particles_ = nullptr;
	KamataEngine::Model* model_E_Particle_ = nullptr;

	int enemyPos = 0;

	int respawnTimer = 120;

	// 敵HPテクスチャハンドル
	uint32_t enemyhpHandle_ = 0;
	// スプライト
	Sprite* enemyhpSprite_ = nullptr;
	uint32_t _enemyhpHandle_ = 0;
	Sprite* _enemyhpSprite_ = nullptr;



	#pragma region 敵の弾
	// 敵の弾
	KamataEngine::Model* modelE_Bullet_ = nullptr;
	// 弾
	std::list<E_Bullet*> E_bullets_;
	// 速度
	KamataEngine::Vector3 E_B_velocity_;
	// 弾の寿命(フレーム数)
	// int playerBulletLifeTime = 20; // 変更可能な左辺値にするためconstを外し型をintに変更    // スペースキーを押して弾を撃つ
	// 敵の弾の生成
	E_Bullet* E_Bullet_ = nullptr;
	void EnemyAttack();
	#pragma endregion



#pragma endregion

	

#pragma region カメラ関係

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;
	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	// カメラコントロール
	CameraController* cameraController_ = nullptr;
	
#pragma endregion




	// 全ての当たり判定
	void CheckAllCollisions();
	
#pragma region フェーズ・フェード

	// ゲームのフェーズ(型)
	enum class Phase
	{
		kFadeIn,       // フェードイン
		kPlay,         // ゲームプレイ
		kDeath,        // プレイヤーのデス演出
		kEnemyDeath,   // 敵のデス演出
		kFadeOut,      // フェードアウト(オーバー)
		kFadeOut2,     // フェードアウト(クリア)
	};

	// ゲームの現在フェーズから開始
	Phase phase_;

	// フェーズの切り替え
	void ChangePhase();

	// フェード
	Fade* fade_ = nullptr;
#pragma endregion

	


	float t = 0.0f;

	//bool useMouseAttack_ = false;

};
