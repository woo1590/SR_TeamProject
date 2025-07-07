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
	HANDLE hFile(nullptr);
	hFile = CreateFileW(saveStage.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(EngineCore::GetInstance()->GetWindowHandle(), "Save Fail", _T("Fail"), MB_OK);
		return;
	}

	DWORD dwByte(0);
	for (auto iter = worldChunks.begin(); iter != worldChunks.end(); ++iter)
		WriteFile(hFile, &iter, sizeof(CHUNK), &dwByte, nullptr);

    CloseHandle(hFile);
    MessageBox(EngineCore::GetInstance()->GetWindowHandle(), "Save Success", _T("Success"), MB_OK);
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