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

Chunk* ChunkManager::CreateChunk(int chunkX, int chunkZ)
{
    auto key = std::make_pair(chunkX, chunkZ);
    auto it = worldChunks.find(key);

    if (it != worldChunks.end())
        return it->second;

    Chunk* newChunk = Chunk::Create(owner->GetObjectManager(), chunkX, chunkZ);
    newChunk->AddRef();
    worldChunks[key] = newChunk;

    return newChunk;
}

void ChunkManager::RemoveChunk(int chunkX, int chunkZ)
{
    auto key = std::make_pair(chunkX, chunkZ);
    auto it = worldChunks.find(key);
    if (it != worldChunks.end())
    {
        Safe_Release(it->second);
        worldChunks.erase(it);
    }
}

void ChunkManager::ClearAllChunks()
{
    for (auto& iter : worldChunks) if (worldChunks.size() > 1) Safe_Release(iter.second);
    for (auto& iter : worldChunks)
    {
        iter.second->InitializeAirBlocks();
        iter.second->BuildChunkFace();
    }
    worldChunks.clear();
}

void ChunkManager::SaveChunk(const std::wstring& saveStage)
{
    HANDLE hFile = CreateFileW(saveStage.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(EngineCore::GetInstance()->GetWindowHandle(), "Save Fail", "Fail", MB_OK);
        return;
    }

    DWORD dwByte(0);
    int chunkCount = worldChunks.size();
    WriteFile(hFile, &chunkCount, sizeof(chunkCount), &dwByte, nullptr);

    for (const auto& pair : worldChunks)
    {
        int chunkX = pair.first.first;
        int chunkZ = pair.first.second;

        WriteFile(hFile, &chunkX, sizeof(int), &dwByte, nullptr);
        WriteFile(hFile, &chunkZ, sizeof(int), &dwByte, nullptr);

        std::vector<SB> nonAirBlocks;

        for (int x = 0; x < CHUNK_SIZE; ++x)
        {
            for (int y = 0; y < CHUNK_HEIGHT; ++y)
            {
                for (int z = 0; z < CHUNK_SIZE; ++z)
                {
                    SB block = pair.second->GetBlock(x, y, z);
                    if (block.Type != StaticBlockType::Air)
                    {
                        nonAirBlocks.push_back(block);
                    }
                }
            }
        }

        int blockCount = nonAirBlocks.size();
        WriteFile(hFile, &blockCount, sizeof(blockCount), &dwByte, nullptr);
        if (blockCount > 0)
            WriteFile(hFile, nonAirBlocks.data(), sizeof(SB) * blockCount, &dwByte, nullptr);
    }

    CloseHandle(hFile);
    // MessageBox(EngineCore::GetInstance()->GetWindowHandle(), "Save Success", "Success", MB_OK);
}

void ChunkManager::LoadChunk(const std::wstring& loadPath)
{
    HANDLE hFile = CreateFileW(loadPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(EngineCore::GetInstance()->GetWindowHandle(), "Load Fail", "Fail", MB_OK);
        return;
    }

    DWORD dwByte = 0;
    int chunkCount = 0;
    if (!ReadFile(hFile, &chunkCount, sizeof(chunkCount), &dwByte, nullptr)) return;

    for (int i = 0; i < chunkCount; ++i)
    {
        int chunkX = 0, chunkZ = 0;
        if (!ReadFile(hFile, &chunkX, sizeof(int), &dwByte, nullptr)) return;
        if (!ReadFile(hFile, &chunkZ, sizeof(int), &dwByte, nullptr)) return;

        int blockCount = 0;
        if (!ReadFile(hFile, &blockCount, sizeof(blockCount), &dwByte, nullptr)) return;

        std::vector<StaticBlockData> blocks(blockCount);
        if (blockCount > 0)
            if (!ReadFile(hFile, blocks.data(), sizeof(StaticBlockData) * blockCount, &dwByte, nullptr)) return;

        Chunk* chunk = Chunk::Create(owner->GetObjectManager(), chunkX, chunkZ);
        chunk->InitializeAirBlocks();
        chunk->SetBlocksFromFlatVector(blocks);
        chunk->BuildChunkFace();
        worldChunks[{chunkX, chunkZ}] = chunk;
    }

    CloseHandle(hFile);
    // MessageBox(EngineCore::GetInstance()->GetWindowHandle(), "Load Success", "Success", MB_OK);
}

Chunk* ChunkManager::GetChunk(int chunkX, int chunkZ)
{
    auto it = worldChunks.find({ chunkX, chunkZ });
    return (it != worldChunks.end()) ? it->second : nullptr;
}

void ChunkManager::Free()
{
    for (auto& [pos, chunkPtr] : worldChunks)
        Safe_Release(chunkPtr);
    
    worldChunks.clear();
}