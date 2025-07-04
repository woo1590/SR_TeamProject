#include "EnginePCH.h"
#include "ChunkManager.h"
#include "Scene.h"
#include "Chunk.h"
#include "StaticBlock.h"
#include "DynamicBlock.h"

ChunkManager::ChunkManager(Scene* owner) : owner(owner)
{
}

ChunkManager::~ChunkManager()
{
}

ChunkManager* ChunkManager::Create(Scene* owner)
{
	ChunkManager* Instance = new ChunkManager(owner);

	if (FAILED(Instance->Ready_ChunkManager()))
	{
		Safe_Release(Instance);
		Instance = nullptr;
	}

	return Instance;
}

HRESULT ChunkManager::Ready_ChunkManager()
{
	return S_OK;
}

void ChunkManager::Activate(int chunkX, int chunkY)
{
	auto iter = worldChunks.find({ chunkX, chunkY });
	if (iter == worldChunks.end())
		return;

	Chunk* chunk = iter->second;
	const CHUNK& chunkData = chunk->GetChunkData();

	if (chunkData.Render)
		return;

	chunk->AddBlock(owner->GetObjectManager(), 0);
	chunk->SetRender(TRUE);
}

void ChunkManager::LoadChunk(int chunkX, int chunkY)
{
	if(worldChunks.contains({ chunkX, chunkY }))
		return;

	// CHUNK data = GenerateChunk(chunkX, chunkY);
	// Chunk* chunk = Chunk::Create(data);
	// worldChunks[{chunkX, chunkY}] = chunk;
}

void ChunkManager::UnLoadChunk(int chunkX, int chunkY)
{
	auto it = worldChunks.find({ chunkX, chunkY });
	if (it == worldChunks.end())
		return;

	Chunk* chunk = it->second;
	chunk->SetRender(FALSE);
	for (auto& Dst : owner->GetObjectManager()->GetObjectList(ObjectType::StaticBlock))
	{
		if (static_cast<StaticBlock*>(Dst)->GetPosition() == chunk->GetChunkData().sBlocks.begin()->Pos)
		{
			owner->GetObjectManager()->RemoveObject(ObjectType::StaticBlock, static_cast<StaticBlock*>(Dst)->GetPosition());
			break;
		}
	}

	Safe_Release(chunk);
	worldChunks.erase(it);
}

void ChunkManager::Free()
{
	for (auto& pair : worldChunks)
		Safe_Release(pair.second);
	worldChunks.clear();
}