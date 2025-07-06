#include "EnginePCH.h"
#include "ChunkManager.h"
#include "Scene.h"
#include "Chunk.h"
#include "StaticBlock.h"
#include "DynamicBlock.h"
#include "EngineCore.h"

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

void ChunkManager::SaveChunk(const std::wstring& saveStage)
{
	HANDLE hFile = CreateFileW(saveStage.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwByte = 0;
	DWORD chunkCount = static_cast<DWORD>(worldChunks.size());
	WriteFile(hFile, &chunkCount, sizeof(DWORD), &dwByte, nullptr);

	for (const auto& [key, chunk] : worldChunks)
	{
		const CHUNK& data = chunk->GetChunkData();

		WriteFile(hFile, &data.chunkX, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &data.chunkZ, sizeof(int), &dwByte, nullptr);

		DWORD sbSize = static_cast<DWORD>(data.sBlocks.size());
		DWORD dbSize = static_cast<DWORD>(data.dBlocks.size());

		WriteFile(hFile, &sbSize, sizeof(DWORD), &dwByte, nullptr);
		for (const SB& sb : data.sBlocks)
			WriteFile(hFile, &sb, sizeof(SB), &dwByte, nullptr);

		WriteFile(hFile, &dbSize, sizeof(DWORD), &dwByte, nullptr);
		for (const DB& db : data.dBlocks)
		{
			WriteFile(hFile, &db, sizeof(DB), &dwByte, nullptr);

			if (db.Type == DynamicBlockType::LeverSwitch)
			{
				for (auto& obj : owner->GetObjectManager()->GetObjectList(ObjectType::DynamicBlock))
				{
					if (obj->GetComponent<TransformComponent>()->GetPosition() == db.Pos)
					{
						int id = static_cast<DynamicBlock*>(obj)->GetID();
						WriteFile(hFile, &id, sizeof(int), &dwByte, nullptr);
						break;
					}
				}
			}
			else if (db.Type == DynamicBlockType::IronCages)
			{
				for (auto& obj : owner->GetObjectManager()->GetObjectList(ObjectType::DynamicBlock))
				{
					if (obj->GetComponent<TransformComponent>()->GetPosition() == db.Pos)
					{
						auto cage = static_cast<DynamicBlock*>(obj);
						int count = cage->GetCount();
						WriteFile(hFile, &count, sizeof(int), &dwByte, nullptr);

						const auto& ids = cage->GetIDVec();
						int size = static_cast<int>(ids.size());
						WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
						if (size > 0)
							WriteFile(hFile, ids.data(), sizeof(int) * size, &dwByte, nullptr);
						break;
					}
				}
			}
		}
	}

	CloseHandle(hFile);
	MessageBox(EngineCore::GetInstance()->GetWindowHandle(), "Save Success", _T("Success"), MB_OK);
}

void ChunkManager::LoadChunk(const std::wstring& loadPath)
{
	HANDLE hFile = CreateFileW(loadPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwByte = 0;
	DWORD chunkCount = 2;
	if (!ReadFile(hFile, &chunkCount, sizeof(DWORD), &dwByte, nullptr)) { CloseHandle(hFile); return; }

	for (DWORD i = 0; i < chunkCount; ++i)
	{
		int chunkX, chunkZ;
		if (!ReadFile(hFile, &chunkX, sizeof(int), &dwByte, nullptr)) break;
		if (!ReadFile(hFile, &chunkZ, sizeof(int), &dwByte, nullptr)) break;

		auto newChunk = Chunk::Create(chunkX, chunkZ);  // 생성자 필요 시 맞게 수정
		CHUNK chunkData = newChunk->GetChunkData();

		chunkData.chunkX = chunkX;
		chunkData.chunkZ = chunkZ;

		// Static Block
		DWORD sbSize = 0;
		if (!ReadFile(hFile, &sbSize, sizeof(DWORD), &dwByte, nullptr)) break;

		for (DWORD s = 0; s < sbSize; ++s)
		{
			SB sb;
			if (!ReadFile(hFile, &sb, sizeof(SB), &dwByte, nullptr)) break;

			auto block = StaticBlock::Create(owner->GetObjectManager(), ObjectType::StaticBlock, sb.Type, sb.Dir);
			block->GetComponent<TransformComponent>()->SetPosition(sb.Pos);
			owner->GetObjectManager()->AddObject(ObjectType::StaticBlock, block);

			chunkData.sBlocks.push_back(sb);
		}

		// Dynamic Block
		DWORD dbSize = 0;
		if (!ReadFile(hFile, &dbSize, sizeof(DWORD), &dwByte, nullptr)) break;

		for (DWORD d = 0; d < dbSize; ++d)
		{
			DB db;
			if (!ReadFile(hFile, &db, sizeof(DB), &dwByte, nullptr)) break;

			Object* dBlock = nullptr;

			if (db.Type == DynamicBlockType::IronCages)
			{
				int count = 0;
				if (!ReadFile(hFile, &count, sizeof(int), &dwByte, nullptr)) break;

				dBlock = DynamicBlock::Create(owner->GetObjectManager(), ObjectType::DynamicBlock, db.Type, db.Dir, count);
				dBlock->GetComponent<TransformComponent>()->SetPosition(db.Pos);

				int vecSize = 0;
				if (!ReadFile(hFile, &vecSize, sizeof(int), &dwByte, nullptr)) break;

				if (vecSize > 0)
				{
					std::vector<int> ids(vecSize);
					if (!ReadFile(hFile, ids.data(), sizeof(int) * vecSize, &dwByte, nullptr)) break;
					for (int id : ids)
						static_cast<DynamicBlock*>(dBlock)->AddID(id);
				}
			}
			else
			{
				dBlock = DynamicBlock::Create(owner->GetObjectManager(), ObjectType::DynamicBlock, db.Type, db.Dir, -1);
				dBlock->GetComponent<TransformComponent>()->SetPosition(db.Pos);

				if (db.Type == DynamicBlockType::LeverSwitch)
				{
					int id = 0;
					if (!ReadFile(hFile, &id, sizeof(int), &dwByte, nullptr)) break;
					static_cast<DynamicBlock*>(dBlock)->SetID(id);
				}
			}

			owner->GetObjectManager()->AddObject(ObjectType::DynamicBlock, dBlock);
			chunkData.dBlocks.push_back(db);
		}

		worldChunks[{chunkX, chunkZ}] = newChunk;
	}

	CloseHandle(hFile);
	MessageBox(EngineCore::GetInstance()->GetWindowHandle(), "Load Success", _T("Success"), MB_OK);
}

void ChunkManager::Free()
{
	for (auto& pair : worldChunks)
		Safe_Release(pair.second);
	worldChunks.clear();
}