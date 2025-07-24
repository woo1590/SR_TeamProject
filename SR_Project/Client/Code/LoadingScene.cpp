#include "pch.h"
#include "LoadingScene.h"
#include "Loader.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "ChunkLoader.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ProgressBar.h"

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
#include "Overlay.h"

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
	loadingUI = LoadingUI::Create(ObjectMgr);
	
	loadingUI->GetComponent<UIRenderer>()->SetCurRenderType(UIRenderType::MainGame);
	loadingUI->SetInfo(nextSceneID);

	ObjectMgr->AddUIObject(loadingUI);
}

void LoadingScene::Update(_float dt)
{
	ObjectMgr->Update(dt); 

    if (loader->IsFinished())
    {
        if (loadingUI->GetCurState() == LoadingState::Static)
        {
            if (!isLoadComplete)
            {
                loadingUI->OnLoadComplete();
                isLoadComplete = true;
            }
            if (EngineCore::GetInstance()->GetInputSystem()->IsKeyPressed(KEY::TAB))
            {
                Scene* nextScene = Village::Create();
                EngineCore::GetInstance()->GetSceneManager()->SetActiveScene(nextScene);
            }
        }
        else
        {
            if (!overlay)
            {
                overlay = Overlay::Create(ObjectMgr, ObjectType::Overlay);
                overlay->SetDuration(1.f);
                overlay->SetFadeIn(false);
                overlay->AddRef();
                ObjectMgr->AddObject(ObjectType::Overlay, overlay);
            }
            else
            {
                if (overlay->IsFinished())
                {

                    Scene* nextScene = nullptr;
                    switch (nextSceneID)
                    {
                    case LOADID::Village:
                    {
                        nextScene = Village::Create();

                    }break;
                    case LOADID::Stage1:
                    {
                        nextScene = Stage1::Create();

                    }break;
                    case LOADID::Stage2:
                    {
                        nextScene = Stage2::Create();

                    }break;
                    }
                    if (nextScene)
                        EngineCore::GetInstance()->GetSceneManager()->SetActiveScene(nextScene);
                }
            }
        }
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
