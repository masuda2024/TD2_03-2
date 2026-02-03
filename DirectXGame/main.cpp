#include <Windows.h>
#include"KamataEngine.h"


#include"Title.h"
#include"Tutorial.h"
#include"Game.h"
#include"Clear.h"
#include"Over.h"


using namespace KamataEngine;

// DirectXCommonインスタンスの取得
DirectXCommon* dxCommon = DirectXCommon::GetInstance();

enum class Scene 
{
	kUnknown = 0,
	kTitle,
	kTutorial,
	kGame,
	kClear,
	kOver,
};
Scene scene = Scene::kUnknown;

void ChangeScene();
void UpdateScene();
void DrawScene();

// タイトルシーンの生成
Title* title = nullptr;

// チュートリアルシーンの生成
Tutorial* tutorial = nullptr;

// ゲームシーンのインスタンス生成
Game* gameScene = nullptr;

// ゲームクリアシーンの生成
Clear* gameClear = nullptr;

// ゲームオーバーシーンの生成
Over* gameOver = nullptr;



uint32_t T_Handle_ = 0;
uint32_t T_Voice_ = 0;

uint32_t G_Handle_ = 0;
uint32_t G_Voice_ = 0;

uint32_t C_Handle_ = 0;
uint32_t C_Voice_ = 0;

uint32_t O_Handle_ = 0;
uint32_t O_Voice_ = 0;





// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	// タイトルバー
	KamataEngine::Initialize(L"2368_回転道");
	// ImGuiManagerインスタンスの取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	// 最初のシーンの初期化
	scene = Scene::kTitle;
	title = new Title;
	tutorial = new Tutorial;
	gameClear = new Clear;
	gameOver = new Over;

	// タイトルシーンの初期化
	title->Initialize();

	// チュートリアルシーンの初期化
	tutorial->Initialize();

	
	// ゲームオーバーシーンの初期化
	gameOver->Initialize();
	//ゲームクリアシーンの初期化
	gameClear->Initialize();



	T_Handle_ = Audio::GetInstance()->LoadWave("Sounds/bgm/Future_1.mp3");
	T_Voice_ = Audio::GetInstance()->PlayWave(T_Handle_, true);

	G_Handle_=Audio::GetInstance()->LoadWave("Sounds/bgm/Shooting_01.mp3");
	

	C_Handle_ = Audio::GetInstance()->LoadWave("Sounds/bgm/Fusion_01.mp3");
	O_Handle_ = Audio::GetInstance()->LoadWave("Sounds/bgm/Springin_Radio_Ending.mp3");




	while (true) 
	{
		// エンジンの更新
		if (KamataEngine::Update()) 
		{
			break;
		}

		// シーン切り替え
		ChangeScene();




		imguiManager->Begin();

		// 現在シーン更新
		UpdateScene();

		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		// 現在シーンの描画
		DrawScene();

		imguiManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// タイトルシーンの解放
	delete title;

	// チュートリアルシーンの解放
	if (tutorial)
	{
		delete tutorial;
	}

	// ゲームシーンの解放
	delete gameScene;

	// ゲームクリアシーンの解放
	delete gameClear;

	// ゲームオーバーシーンの解放
	delete gameOver;

	// nullptrの代入

	gameScene = nullptr;

	KamataEngine::Finalize();


	return 0;
}

void UpdateScene()
{
	switch (scene)
	{
	case Scene::kTitle:
		title->Update();
		break;

	case Scene::kTutorial:
		tutorial->Update();
		break;
		
	case Scene::kGame:
		gameScene->Update();
		break;

	case Scene::kClear:
		gameClear->Update();
		break;

	case Scene::kOver:
		gameOver->Update();
		break;
	}
}

void ChangeScene() 
{
	switch (scene)
	{
	case Scene::kTitle:

		if (title->IsFinishedT())
		{
			
			// シーンの変更
			scene = Scene::kTutorial;
			// 新シーンの生成と初期化
			tutorial = new Tutorial();
			tutorial->Initialize();

			
		}
		
		break;
	case Scene::kTutorial:
		if (tutorial->IsFinishedTU())
		{

			// 音声停止
			Audio::GetInstance()->StopWave(T_Voice_);


			// シーンの変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete tutorial;
			tutorial = nullptr;
			title = nullptr;
			// 新シーンの生成と初期化
			gameScene = new Game();
			gameScene->Initialize();

			// ゲームシーンの音楽を再生
			G_Voice_ = Audio::GetInstance()->PlayWave(G_Handle_, true);

		}
		break;

	case Scene::kGame:

		// ゲームシーンでバリアが破壊された場合
		if (gameScene->IsFinishedGAME()) 
		{

			// 音声停止
			Audio::GetInstance()->StopWave(G_Voice_);

			// シーンの変更
			scene = Scene::kOver;

			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// クリアオーバーの生成
			gameOver = new Over;
			// クリアオーバーの初期化
			gameOver->Initialize();

			// ゲームオーバーシーンの音楽を再生
			O_Voice_ = Audio::GetInstance()->PlayWave(O_Handle_, true);

		} else if (gameScene->IsFinishedGAME2())
		{
			// 音声停止
			Audio::GetInstance()->StopWave(G_Voice_);

			// プレイヤーが敵を倒した場合
			// シーンの変更
			scene = Scene::kClear;

			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;

			// クリアシーンの生成
			gameClear = new Clear;
			// クリアシーンの初期化
			gameClear->Initialize();

			// ゲームクリアシーンの音楽を再生
			C_Voice_ = Audio::GetInstance()->PlayWave(C_Handle_, true);
		}
		break;

	case Scene::kClear:

		if (gameClear->IsFinishedC()) 
		{
			// 音声停止
			Audio::GetInstance()->StopWave(C_Voice_);

			// シーンの変更
			scene = Scene::kTitle;

			// 旧シーンの解放
			delete gameClear;
			gameClear = nullptr;

			// タイトルシーンの生成
			title = new Title;
			// タイトルシーンの初期化
			title->Initialize();

			// タイトルの音楽を再生
			T_Voice_ = Audio::GetInstance()->PlayWave(T_Handle_, true);
		}
		break;

	case Scene::kOver:

		if (gameOver->IsFinishedO()) 
		{

			// 音声停止
			Audio::GetInstance()->StopWave(O_Voice_);

			// シーンの変更
			scene = Scene::kTitle;

			// 旧シーンの解放
			delete gameOver;
			gameOver = nullptr;

			// タイトルシーンの生成
			title = new Title;
			// タイトルシーンの初期化
			title->Initialize();

			// タイトルの音楽を再生
			T_Voice_ = Audio::GetInstance()->PlayWave(T_Handle_, true);
		}
		break;
	}
}

void DrawScene()
{

	switch (scene)
	{
	case Scene::kTitle:
		title->Draw();
		break;

	case Scene::kTutorial:
		tutorial->Draw();
		break;
		
	case Scene::kGame:
		gameScene->Draw();
		break;

	case Scene::kClear:
		gameClear->Draw();
		break;

	case Scene::kOver:
		gameOver->Draw();
		break;
	}
}