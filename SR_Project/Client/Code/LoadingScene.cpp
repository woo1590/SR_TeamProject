#include "pch.h"
#include "LoadingScene.h"
#include "Loader.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "SceneManager.h"
#include "ObjectManager.h"

//scene
#include "TestScene.h"
#include "LogoScene.h"

LoadingScene::LoadingScene(LOADID loadId)
	:nextSceneID(loadId)
{
}

LoadingScene::~LoadingScene()
{
}

LoadingScene* LoadingScene::Create(LOADID loadId)
{
	LoadingScene* Instance = new LoadingScene(loadId);

	return Instance;
}

void LoadingScene::Load()
{
	loader = Loader::Create(nextSceneID);
}

void LoadingScene::Update(_float dt)
{
	if (loader->IsFinished())	//Load complete
	{
		auto Input = EngineCore::GetInstance()->GetInputSystem();
		Scene* nextScene = nullptr;	

		if (Input->IsKeyPressed(SPACE))
		{
			switch (nextSceneID)
			{
			case LOADID::TestScene:
				nextScene = TestScene::Create();
				break;
			default:
				break;
			}

			if (nextScene)
				EngineCore::GetInstance()->GetSceneManager()->SetActiveScene(nextScene);
		}
	}
}

void LoadingScene::Late_Update(_float dt)
{
}

void LoadingScene::Unload()
{
	Safe_Release(loader);
}

void LoadingScene::Free()
{
	Scene::Free();
}
