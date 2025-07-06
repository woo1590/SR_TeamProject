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

void ChunkManager::UpdateChunk(const _vec3& playerPos)
{
    int playerChunkX = static_cast<int>(floor(playerPos.x / CHUNK_SIZE));
    int playerChunkY = static_cast<int>(floor(playerPos.z / CHUNK_SIZE));

    std::unordered_set<std::pair<int, int>, PairHash> neededChunks;

    for (int x = playerChunkX - renderDistance; x <= playerChunkX + renderDistance; ++x)
    {
        for (int y = playerChunkY - renderDistance; y <= playerChunkY + renderDistance; ++y)
        {
            neededChunks.emplace(x, y);
            if (worldChunks.find({ x, y }) == worldChunks.end())
            {
                CreateChunk(x, y);
            }
        }
    }

    for (auto it = worldChunks.begin(); it != worldChunks.end(); )
    {
        if (neededChunks.find(it->first) == neededChunks.end())
        {
            Safe_Release(it->second);
            it = worldChunks.erase(it);
        }
        else
            ++it;
    }
}

void ChunkManager::RenderChunk(const _vec3& playerPos)
{
    int playerChunkX = static_cast<int>(floor(playerPos.x / CHUNK_SIZE));
    int playerChunkY = static_cast<int>(floor(playerPos.z / CHUNK_SIZE));

    for (int x = playerChunkX - renderDistance; x <= playerChunkX + renderDistance; ++x)
    {
        for (int y = playerChunkY - renderDistance; y <= playerChunkY + renderDistance; ++y)
        {
            auto it = worldChunks.find({ x, y });
            if (it != worldChunks.end())
            {
                it->second->Render();
            }
        }
    }
}

void ChunkManager::Free()
{
	for (auto& pair : worldChunks)
		Safe_Release(pair.second);
	worldChunks.clear();
}