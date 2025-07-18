#include "pch.h"
#include "LoadingScene.h"
#include "Loader.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "ChunkLoader.h"
#include "UIRenderer.h"

//scene
#include "TestScene.h"
#include "LogoScene.h"
#include "Village.h"
#include "Stage1.h"
#include "Stage2.h"

//Objects
#include "LoadingUI.h"
#include "Cursor.h"
#include "LoadingStone.h"

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
	ObjectMgr = ObjectManager::Create(this);

	loader = Loader::Create(nextSceneID);

	loadingUI = LoadingUI::Create(nullptr);
	loadingStone = LoadingStone::Create(nullptr);

	switch (nextSceneID)
	{
	case LOADID::Stage1:
		loadingUI->GetComponent<UIRenderer>()->SetTexture(L"loadingscene_stage1");
		loadingUI->SetVisible(false);
		break;

	case LOADID::Stage2:
		loadingUI->GetComponent<UIRenderer>()->SetTexture(L"loadingscene_stage1");
		loadingUI->SetVisible(false);
		break;

	default: 
		loadingUI->GetComponent<UIRenderer>()->SetTexture(L"loadingscene");
		break;
	}
	
	ObjectMgr->AddUIObject(loadingUI);
	ObjectMgr->AddUIObject(loadingStone);
}

void LoadingScene::Update(_float dt)
{
	ObjectMgr->Update(dt);

	if (loader->IsFinished())	//Load complete
	{
		auto Input = EngineCore::GetInstance()->GetInputSystem();
		Scene* nextScene = nullptr;	

		switch (nextSceneID)
		{
		case LOADID::TestScene:
			nextScene = TestScene::Create();
			break;
		case LOADID::Village:
			nextScene = Village::Create();
			break;
		case LOADID::Stage1:
			nextScene = Stage1::Create();
			break;
		case LOADID::Stage2:
			nextScene = Stage2::Create();
			break;
		default:
			break;
		}

		if (nextScene)
			EngineCore::GetInstance()->GetSceneManager()->SetActiveScene(nextScene);
	}
}

void LoadingScene::Late_Update(_float dt)
{
	ObjectMgr->Late_Update(dt);
}

void LoadingScene::Unload()
{
	Safe_Release(loader);
	Safe_Release(ObjectMgr);
}

void LoadingScene::Free()
{
	Scene::Free();
}
