#include "EnginePCH.h"
#include "CameraManager.h"
#include "Object.h"
#include "CameraComponent.h"
#include "EngineCore.h"
#include "RenderSystem.h"
#include "Scene.h"
#include "EventSystem.h"

CameraManager::CameraManager(Scene* owner)
	:owner(owner)
{
}

CameraManager::~CameraManager()
{
}

CameraManager* CameraManager::Create(Scene* owner)
{
	CameraManager* Instance = new CameraManager(owner);

	if (FAILED(Instance->Ready_CameraManager()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT CameraManager::Ready_CameraManager()
{
	return S_OK;
}

void CameraManager::AddCamera(const std::wstring& key, Object* camera)
{
	CameraMap[key] = camera;
	camera->AddRef();
}

bool CameraManager::SetMainCamera(const std::wstring& key)
{
	auto iter = CameraMap.find(key);

	if (iter != CameraMap.end())
	{
		MainCamera = iter->second;

		EngineCore::GetInstance()->GetRenderSystem()->SetCamera(MainCamera);
		return true;
	}
	else
		return false;
}

CameraComponent* CameraManager::GetMainCamera()
{
	if (MainCamera)
		return MainCamera->GetComponent<CameraComponent>();
}

void CameraManager::Free()
{
	for (auto& [key, camera] : CameraMap)
		Safe_Release(camera);

	CameraMap.clear();
}
