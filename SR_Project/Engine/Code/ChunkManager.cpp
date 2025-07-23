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
    Free();
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
    if (it != worldChunks.end()) return it->second;

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
    for (auto& iter : worldChunks)
    {
        iter.second->ClearAlpha();
        iter.second->InitializeAirBlocks();
        iter.second->BuildChunkFace();
    }

    for (auto& iter : worldChunks)
        if (worldChunks.size() > 1)
            Safe_Release(iter.second);
    
    worldChunks.clear();
}

void ChunkManager::SaveChunk(const std::wstring& saveStage)
{
    HANDLE hFile = CreateFileW(saveStage.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) return;

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

        const auto& alphaList = pair.second->GetAlphaBlocks();
        int alphaCount = static_cast<int>(alphaList.size());
        WriteFile(hFile, &alphaCount, sizeof(int), &dwByte, nullptr);

        for (auto obj : alphaList)
        {
            auto tf = obj->GetComponent<TransformComponent>();
            _vec3 pos = tf->GetPosition();

            auto* alphaBlock = static_cast<StaticBlock*>(obj);
            StaticBlockData data =
            {
                pos,
                alphaBlock->GetType(),
                sAEnd,
                sREnd,
                Alpha
            };
            WriteFile(hFile, &data, sizeof(StaticBlockData), &dwByte, nullptr);
        }
    }

    CloseHandle(hFile);
}

void ChunkManager::LoadChunk(const std::wstring& loadPath, bool isEditor)
{
    HANDLE hFile = CreateFileW(loadPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) return;

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

        int alphaCount(0);
        if (!ReadFile(hFile, &alphaCount, sizeof(int), &dwByte, nullptr)) return;

        for (int j = 0; j < alphaCount; ++j)
        {
            StaticBlockData data{};
            if (!ReadFile(hFile, &data, sizeof(StaticBlockData), &dwByte, nullptr)) return;

            auto alphaObj = StaticBlock::Create(owner->GetObjectManager(), ObjectType::StaticBlock, data.Type, data.Axis, data.Rot, data.Usage);
            alphaObj->GetComponent<TransformComponent>()->SetPosition(data.Pos);
            owner->GetObjectManager()->AddObject(ObjectType::AlphaBlock, alphaObj);

            chunk->AddAlphaBlock(alphaObj);
            owner->GetStaticBlocks().push_back(data);
        }

        chunk->AddRef();
        worldChunks[{chunkX, chunkZ}] = chunk;
    }

    for (auto& [pair, chunk] : worldChunks)
    {
        chunk->BuildChunkFace();
        chunk->BuildCollisionBlock();
    }

    CloseHandle(hFile);
}

void ChunkManager::IsChunkBoundary(_vec3 playerPos)
{
    int chunkX = static_cast<int>(floor(playerPos.x / CHUNK_SIZE));
    int chunkZ = static_cast<int>(floor(playerPos.z / CHUNK_SIZE));

    if (chunkX != preChunkX || chunkZ != preChunkZ)
    {
        preChunkX = chunkX;
        preChunkZ = chunkZ;
        UpdateRenderChunk(chunkX, chunkZ, chunkRange);
    }
}

void ChunkManager::UpdateRenderChunk(int playerChunkX, int playerChunkZ, int count)
{
    if (worldChunks.empty()) return;

    for (auto& chunk : worldChunks)
        chunk.second->SetChunkRender(FALSE);

    for (int x = -count; x <= count; ++x)
    {
        for (int z = -count; z <= count; ++z)
        {
            if (playerChunkX + x < 0 || playerChunkZ + z < 0) continue;
            if (GetChunk(playerChunkX + x, playerChunkZ + z)) GetChunk(playerChunkX + x, playerChunkZ + z)->SetChunkRender(TRUE);
        }
    }
}

Chunk* ChunkManager::GetChunk(int chunkX, int chunkZ)
{
    auto it = worldChunks.find({ chunkX, chunkZ });
    return (it != worldChunks.end()) ? it->second : nullptr;
}

void ChunkManager::SetChunk(std::unordered_map<std::pair<int, int>, Chunk*, PairHash>& chunks)
{
    worldChunks = chunks;
    for (auto& chunk : worldChunks)
    {
        chunk.second->SetOwner(owner->GetObjectManager());
    }
}

void ChunkManager::CreateMiniMapChunk(int chunkX, int chunkZ, Chunk* chunk, SceneID sceneID)
{
    MiniMapChunkData& data = miniMapChunks[{chunkX, chunkZ}];
    chunk->CreatePathMap(data.path, sceneID);
}

bool ChunkManager::GetMiniMapChunk(int chunkX, int chunkZ, MINIMAP& outData)
{
    auto it = miniMapChunks.find({ chunkX, chunkZ });

    if (it != miniMapChunks.end())
    {
        outData = it->second;
        return true;
    }

    return false;
}

void ChunkManager::Free()
{
    for (auto& [pos, chunkPtr] : worldChunks)
        Safe_Release(chunkPtr);
    
    worldChunks.clear();
}