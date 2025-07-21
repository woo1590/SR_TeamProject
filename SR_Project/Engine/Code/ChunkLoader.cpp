#include "EnginePCH.h"
#include "ChunkLoader.h"
#include "Chunk.h"
#include "StaticBlock.h"
#include "DynamicBlock.h"

ChunkLoader::ChunkLoader()
{
}

ChunkLoader::~ChunkLoader()
{
}

ChunkLoader* ChunkLoader::Create()
{
	ChunkLoader* Instance = new ChunkLoader;

	if (FAILED(Instance->Ready_ChunkLoader()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT ChunkLoader::Ready_ChunkLoader()
{
	return S_OK;
}

HRESULT ChunkLoader::LoadDB(const std::wstring& loadPath)
{
	
	return S_OK;
}

HRESULT ChunkLoader::LoadChunk(const std::wstring& loadPath)
{
    worldChunks.clear();

    HANDLE hFile = CreateFileW(loadPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) return E_FAIL;

    DWORD dwByte = 0;
    int chunkCount = 0;
    if (!ReadFile(hFile, &chunkCount, sizeof(chunkCount), &dwByte, nullptr)) return E_FAIL;

    for (int i = 0; i < chunkCount; ++i)
    {
        int chunkX = 0, chunkZ = 0;
        if (!ReadFile(hFile, &chunkX, sizeof(int), &dwByte, nullptr)) return E_FAIL;
        if (!ReadFile(hFile, &chunkZ, sizeof(int), &dwByte, nullptr)) return E_FAIL;

        int blockCount = 0;
        if (!ReadFile(hFile, &blockCount, sizeof(blockCount), &dwByte, nullptr)) return E_FAIL;

        std::vector<StaticBlockData> blocks(blockCount);
        if (blockCount > 0)
            if (!ReadFile(hFile, blocks.data(), sizeof(StaticBlockData) * blockCount, &dwByte, nullptr)) return E_FAIL;

        Chunk* chunk = Chunk::Create(nullptr, chunkX, chunkZ);  //청크 생성후 공기블럭 초기화, 블럭 생성
        chunk->InitializeAirBlocks();
        chunk->SetBlocksFromFlatVector(blocks);

        int alphaCount(0);
        if (!ReadFile(hFile, &alphaCount, sizeof(int), &dwByte, nullptr)) return E_FAIL;

        for (int j = 0; j < alphaCount; ++j)
        {
            StaticBlockData data{};
            if (!ReadFile(hFile, &data, sizeof(StaticBlockData), &dwByte, nullptr)) return E_FAIL;

            auto alphaObj = StaticBlock::Create(nullptr, ObjectType::StaticBlock, data.Type, data.Axis, data.Rot, data.Usage);
            alphaObj->GetComponent<TransformComponent>()->SetPosition(data.Pos);
            //owner->GetObjectManager()->AddObject(ObjectType::AlphaBlock, alphaObj);

            chunk->AddAlphaBlock(alphaObj);
            //owner->GetStaticBlocks().push_back(data);
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

	return S_OK;
}

void ChunkLoader::Free()
{
}
