#include "EnginePCH.h"
#include "UIManager.h"

UIManager* UIManager::Create()
{
	UIManager* instance = new UIManager;

	if (FAILED(instance->Ready_UIManager()))
	{
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT UIManager::Ready_UIManager()
{
	return S_OK;
}
