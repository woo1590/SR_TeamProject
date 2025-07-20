#include "pch.h"
#include "GameManager.h"

IMPLEMENT_SINGLETON(GameManager);

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

HRESULT GameManager::Ready_GameManager()
{
	clearScenes.resize(static_cast<_uint>(LOADID::Count), false);

	return S_OK;
}

void GameManager::ClearScene(LOADID sceneID)
{
	_uint idx = static_cast<_uint>(sceneID);

	clearScenes[idx] = true;
}

_bool GameManager::IsSceneClear(LOADID sceneID)
{
	return clearScenes[static_cast<_uint>(sceneID)];
}

void GameManager::Free()
{

}
