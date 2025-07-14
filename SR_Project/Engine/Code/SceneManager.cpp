#include "EnginePCH.h"
#include "SceneManager.h"
#include "EngineCore.h"
#include "RenderSystem.h"
#include "ImGuiManager.h"
#include "Scene.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

SceneManager* SceneManager::Create()
{
    SceneManager* Instance = new SceneManager;
    if (FAILED(Instance->Ready_SceneManager()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT SceneManager::Ready_SceneManager()
{
    ActiveScene = nullptr;

    return S_OK;
}

void SceneManager::Update(float dt)
{
    ActiveScene->Update(dt);
}

void SceneManager::Late_Update(float dt)
{
    ActiveScene->Late_Update(dt);
}

Scene* SceneManager::GetActiveScene() const
{
    return ActiveScene;
}

void SceneManager::SetActiveScene(Scene* nextScene)
{
    if (ActiveScene)
    {
        ActiveScene->Unload();
        Safe_Release(ActiveScene);
        EngineCore::GetInstance()->GetRenderSystem()->ClearSystem();
#ifdef USE_IMGUI
        EngineCore::GetInstance()->GetImGuiManager()->ClearWindow();
#endif
    }

    ActiveScene = nextScene;
    ActiveScene->Load();
}

void SceneManager::Free()
{
    if (ActiveScene)
    {
        ActiveScene->Unload();
        Safe_Release(ActiveScene);
    }
}
