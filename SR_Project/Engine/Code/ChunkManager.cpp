#include "EnginePCH.h"
#include "ChunkManager.h"
#include "EngineCore.h"

#include "Scene.h"
#include "Chunk.h"
#include "StaticBlock.h"

ChunkManager::ChunkManager(Scene* owner) : owner(owner)
{
}

ChunkManager::~ChunkManager()
{
}

ChunkManager* ChunkManager::Create(Scene* owner)
{
	ChunkManager* Instance = new ChunkManager(owner);
	return Instance;
}

void ChunkManager::SaveChunk(const std::wstring& saveStage)
{
	
}

void ChunkManager::LoadChunk(const std::wstring& loadPath)
{
	
}

Chunk* ChunkManager::CreateChunk(int chunkX, int chunkY)
{
    auto key = std::make_pair(chunkX, chunkY);
    auto it = worldChunks.find(key);
    if (it != worldChunks.end())
        return it->second;

    Chunk* newChunk = Chunk::Create(chunkX, chunkY);
    worldChunks[key] = newChunk;
    return newChunk;
}

void ChunkManager::RemoveChunk(int chunkX, int chunkY)
{
    auto key = std::make_pair(chunkX, chunkY);
    auto it = worldChunks.find(key);
    if (it != worldChunks.end())
    {
        Safe_Release(it->second);
        worldChunks.erase(it);
    }
}

void ChunkManager::Free()
{
	for (auto& pair : worldChunks)
		Safe_Release(pair.second);
	worldChunks.clear();
}