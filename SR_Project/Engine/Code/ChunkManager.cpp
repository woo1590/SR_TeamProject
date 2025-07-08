#include "EnginePCH.h"
#include "ChunkManager.h"
#include "EngineCore.h"

#include "Scene.h"
#include "Chunk.h"
#include "StaticBlock.h"

ChunkManager::ChunkManager(Scene* owner)
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

void ChunkManager::Update(_float dt)
{
    for (const auto& [key, chunk] : worldChunks)
        chunk->Update(dt);
}

void ChunkManager::SaveChunk(const std::wstring& saveStage)
{
}

void ChunkManager::LoadChunk(const std::wstring& loadStage)
{
}

Chunk* ChunkManager::CreateChunk(int chunkX, int chunkZ)
{
    ChunkCoord coord{ chunkX, chunkZ };

    auto it = worldChunks.find(coord);
    if (it != worldChunks.end())
        return it->second;

    Chunk* newChunk = Chunk::Create(nullptr, ObjectType::Chunk, chunkX, chunkZ);
    if (!newChunk)
        return nullptr;

    worldChunks.emplace(coord, newChunk);

    return newChunk;
}

void ChunkManager::Free()
{
}
