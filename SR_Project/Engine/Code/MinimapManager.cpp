#include "EnginePCH.h"
#include "Scene.h"
#include "MinimapManager.h"
#include "MinimapData.h"
#include "MinimapMesh.h"
#include "ChunkManager.h"

MinimapManager::MinimapManager(Scene* owner) : owner(owner)
{
}

MinimapManager::~MinimapManager()
{
    Free();
}

MinimapManager* MinimapManager::Create(Scene* owner)
{
    MinimapManager* instance = new MinimapManager(owner);
    if (!instance) return nullptr;
    return instance;
}

bool MinimapManager::LoadMiniMap(const std::wstring& filepath)
{
    ClearMiniMap();

    MinimapData = MinimapData::Create(owner->GetObjectManager());
    if (!MinimapData) return false;

    if (!MinimapData->LoadFromMap(filepath))
    {
        Safe_Release(MinimapData);
        return false;
    }

    MinimapMesh = MinimapMesh::Create();
    if (!MinimapMesh)
    {
        Safe_Release(MinimapData);
        return false;
    }

    if (FAILED(MinimapMesh->Ready_Mesh()))
    {
        Safe_Release(MinimapData);
        Safe_Release(MinimapMesh);
        return false;
    }

    return true;
}

bool MinimapManager::GenerateFromChunks(ChunkManager* chunkManager)
{
    ClearMiniMap();

    MinimapData = MinimapData::Create(owner->GetObjectManager());
    if (!MinimapData) return false;

    if (!MinimapData->GenerateFromChunks(chunkManager))
    {
        Safe_Release(MinimapData);
        return false;
    }

    MinimapMesh = MinimapMesh::Create();
    if (!MinimapMesh)
    {
        Safe_Release(MinimapData);
        return false;
    }

    if (FAILED(MinimapMesh->Ready_Mesh()))
    {
        Safe_Release(MinimapData);
        Safe_Release(MinimapMesh);
        return false;
    }

    return true;
}

void MinimapManager::ClearMiniMap()
{
    Safe_Release(MinimapData);
    Safe_Release(MinimapMesh);
}

void MinimapManager::Free()
{
    ClearMiniMap();
    owner = nullptr;
}