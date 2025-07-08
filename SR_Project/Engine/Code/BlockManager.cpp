#include "EnginePCH.h"
#include "BlockManager.h"

#include "EngineCore.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "ChunkManager.h"

#include "Scene.h"
#include "CubeMesh.h"
#include "StaticBlock.h"
#include "DynamicBlock.h"
#include "TransformComponent.h"

BlockManager::BlockManager(Scene* owner) : owner(owner)
{
}

BlockManager::~BlockManager()
{
}

BlockManager* BlockManager::Create(Scene* owner)
{
	BlockManager* Instance = new BlockManager(owner);
	return Instance;
}

void BlockManager::LoadTexture()
{
	auto cube = CubeMesh::Create();
	auto resource = EngineCore::GetInstance()->GetResourceManager();

	resource->LoadMesh("Cube_Mesh", cube);
	resource->LoadShader("../../Client/Resource/Shader/BasicShader.fx","BasicShader");
	resource->LoadMaterial("../../Client/Resource/Material/Blocks.json");
}

void BlockManager::SaveStage(const char* saveStage)
{
	HANDLE hFile(nullptr);
	string path = "../../Reference/MapData/"; path += saveStage; path += ".dat";

	int len(MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, NULL, 0));
	wstring wpath(len, 0);
	MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, &wpath[0], len);

	hFile = CreateFileW(wpath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(EngineCore::GetInstance()->GetWindowHandle(), "Save Fail", _T("Fail"), MB_OK);
		return;
	}

	DWORD dwByte(0);
	DWORD SBSize(owner->GetStaticBlocks().size());
	DWORD DBSize(owner->GetDynamicBlocks().size());
	WriteFile(hFile, &SBSize, sizeof(DWORD), &dwByte, nullptr);
	WriteFile(hFile, &DBSize, sizeof(DWORD), &dwByte, nullptr);
	for (auto& block : owner->GetStaticBlocks()) WriteFile(hFile, &block, sizeof(SB), &dwByte, nullptr);
	for (auto& block : owner->GetDynamicBlocks())
	{
		WriteFile(hFile, &block, sizeof(DB), &dwByte, nullptr);

		switch (block.Type)
		{
		case DynamicBlockType::LeverSwitch:
		{
			for (auto& obj : owner->GetObjectManager()->GetObjectList(ObjectType::DynamicBlock))
			{
				if (obj->GetComponent<TransformComponent>()->GetPosition() == block.Pos)
				{
					int id = static_cast<DynamicBlock*>(obj)->GetID();
					WriteFile(hFile, &id, sizeof(int), &dwByte, nullptr);
					break;
				}
			}
			break;
		}
		case DynamicBlockType::IronCages:
		{
			for (auto& obj : owner->GetObjectManager()->GetObjectList(ObjectType::DynamicBlock))
			{
				if (obj->GetComponent<TransformComponent>()->GetPosition() == block.Pos)
				{
					auto dyn = static_cast<DynamicBlock*>(obj);

					int count = dyn->GetCount();
					WriteFile(hFile, &count, sizeof(int), &dwByte, nullptr);

					auto ids = dyn->GetIDVec();
					int size = static_cast<int>(ids.size());
					WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
					if (size > 0)
						WriteFile(hFile, ids.data(), sizeof(int) * size, &dwByte, nullptr);
					break;
				}
			}
			break;
		}
		}
	}

	CloseHandle(hFile);
	MessageBox(EngineCore::GetInstance()->GetWindowHandle(), "Save Success", _T("Success"), MB_OK);
}

void BlockManager::LoadStage(const char* loadStage)
{
	HANDLE hFile(nullptr);
	string path = "../../Reference/MapData/"; path += loadStage; path += ".dat";

	int len(MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, NULL, 0));
	wstring wpath(len, 0);
	MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, &wpath[0], len);

	hFile = CreateFileW(wpath.c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(EngineCore::GetInstance()->GetWindowHandle(), "Load Fail", _T("Fail"), MB_OK);
		return;
	}

	owner->GetObjectManager()->ClearList(ObjectType::StaticBlock); owner->GetObjectManager()->GetObjectList(ObjectType::StaticBlock).clear();
	owner->GetObjectManager()->ClearList(ObjectType::DynamicBlock); owner->GetObjectManager()->GetObjectList(ObjectType::DynamicBlock).clear();

	DWORD dwByte(0), dwSBTot(0), dwDBTot(0);
	DWORD SBSize(0), DBSize(0);
	SB newSBlock; DB newDBlock;

	if (!ReadFile(hFile, &SBSize, sizeof(DWORD), &dwByte, nullptr)) return;
	if (!ReadFile(hFile, &DBSize, sizeof(DWORD), &dwByte, nullptr)) return;

	for (DWORD i = 0; i < SBSize; ++i)
	{
		if (!ReadFile(hFile, &newSBlock, sizeof(SB), &dwByte, nullptr)) return;

		auto sBlock = StaticBlock::Create(owner->GetObjectManager(), ObjectType::StaticBlock, newSBlock.Type, newSBlock.Axis, newSBlock.Rot, newSBlock.Usage);
		sBlock->GetComponent<TransformComponent>()->SetPosition(newSBlock.Pos);

		owner->GetObjectManager()->AddObject(ObjectType::StaticBlock, sBlock);
		owner->GetStaticBlocks().push_back(newSBlock);
	}

	for (DWORD i = 0; i < DBSize; ++i)
	{
		if (!ReadFile(hFile, &newDBlock, sizeof(DB), &dwByte, nullptr)) return;

		Object* dBlock = nullptr;

		if (newDBlock.Type == DynamicBlockType::IronCages)
		{
			int count = 0;
			if (!ReadFile(hFile, &count, sizeof(int), &dwByte, nullptr)) return;

			dBlock = DynamicBlock::Create(owner->GetObjectManager(), ObjectType::DynamicBlock, newDBlock.Type, newDBlock.Col, newDBlock.Rot, count);
			dBlock->GetComponent<TransformComponent>()->SetPosition(newDBlock.Pos);

			int vecSize = 0;
			if (!ReadFile(hFile, &vecSize, sizeof(int), &dwByte, nullptr)) return;
			if (vecSize > 0)
			{
				std::vector<int> ids(vecSize);
				if (!ReadFile(hFile, ids.data(), sizeof(int) * vecSize, &dwByte, nullptr)) return;
				for (int id : ids)
					static_cast<DynamicBlock*>(dBlock)->AddID(id);
			}
		}
		else
		{
			dBlock = DynamicBlock::Create(owner->GetObjectManager(), ObjectType::DynamicBlock, newDBlock.Type, newDBlock.Col, newDBlock.Rot, -1);
			dBlock->GetComponent<TransformComponent>()->SetPosition(newDBlock.Pos);

			if (newDBlock.Type == DynamicBlockType::LeverSwitch)
			{
				int id = 0;
				if (!ReadFile(hFile, &id, sizeof(int), &dwByte, nullptr)) return;
				static_cast<DynamicBlock*>(dBlock)->SetID(id);
			}
		}

		owner->GetObjectManager()->AddObject(ObjectType::DynamicBlock, dBlock);
		owner->GetDynamicBlocks().push_back(newDBlock);
	}

	CloseHandle(hFile);
	MessageBox(EngineCore::GetInstance()->GetWindowHandle(), "Load Success", _T("Success"), MB_OK);
}

void BlockManager::SaveChunk(const char* saveStage)
{
	std::string path = "../../Reference/MapData/";
	path += saveStage;
	path += ".chunkmap";

	int len = MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, nullptr, 0);
	std::wstring wpath(len, 0);
	MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, &wpath[0], len);

	owner->GetChunkManager()->SaveChunk(wpath);
}

void BlockManager::LoadChunk(const char* loadStage)
{
	std::string path = "../../Reference/MapData/";
	path += loadStage;
	path += ".chunkmap";

	int len = MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, nullptr, 0);
	std::wstring wpath(len, 0);
	MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, &wpath[0], len);

	owner->GetChunkManager()->LoadChunk(wpath);
}

Scene* BlockManager::GetOwner() const
{
	return owner;
}

void BlockManager::Free()
{
}