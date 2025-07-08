#include "pch.h"
#include "EditScene.h"
#include "EngineCore.h"

//system
#include "ObjectManager.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "ImGuiManager.h"
#include "CameraManager.h"
#include "CollisionSystem.h"
#include "BlockManager.h"
#include "ChunkManager.h"

//object
#include "Camera.h"
#include "StaticBlock.h"
#include "DynamicBlock.h"

//component
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"

//object
#include "CubeMesh.h"
#include "Material.h"
#include "GraphicDevice.h"
#include "Lever.h"
#include "Chunk.h"
#include "IronCage.h"

#include "TerrainCreater.h"

EditScene::EditScene()
{
}

EditScene::~EditScene()
{
	Free();
}

EditScene* EditScene::Create()
{
	EditScene* Instance = new EditScene;
	return Instance;
}

void EditScene::Load()
{
#ifdef USE_IMGUI
	EngineCore::GetInstance()->GetImGuiManager()->RegisterWindow(L"MapTool", [this]() {this->ImGui_Main();});
#endif

	CollisionSys = CollisionSystem::Create(this);
	BlockMgr = BlockManager::Create(this);
	ChunkMgr = ChunkManager::Create(this);

	BlockMgr->LoadTexture();
	SB baseBlock{ {0, 0, 0}, StaticBlockType::Dirt, StaticBlockAxis::sAY };
	staticBlocks.push_back(baseBlock);

	ObjectMgr = ObjectManager::Create(this);
	auto cam = Camera::Create(ObjectMgr, ObjectType::Camera);
	ObjectMgr->AddObject(ObjectType::Camera, cam);
	ObjectMgr->AddObject(ObjectType::StaticBlock, StaticBlock::Create(ObjectMgr, ObjectType::StaticBlock, StaticBlockType::Dirt, StaticBlockAxis::sAY, StaticBlockRot::sREnd, StaticBlockUsage::Basic));

	CameraMgr = CameraManager::Create(this);
	CameraMgr->AddCamera(L"ToolCam", cam);
	CameraMgr->SetMainCamera(L"ToolCam");
}

void EditScene::Update(float dt)
{
	ObjectMgr->Update(dt);

	auto Input = EngineCore::GetInstance()->GetInputSystem();
	if (Input->IsKeyPressed(LBUTTON))
	{
		_vec3 rayOrigin, rayDir;
		MakePickingRay(rayOrigin, rayDir);
		OnLeftClick(rayOrigin, rayDir);
	}
	if (Input->IsKeyPressed(RBUTTON))
	{
		_vec3 rayOrigin, rayDir;
		MakePickingRay(rayOrigin, rayDir);
		OnRightClick(rayOrigin, rayDir);
	}
	if (Input->IsKeyPressed(Q))
	{
		for (auto& Dst : ObjectMgr->GetObjectList(ObjectType::DynamicBlock))
		{
			if (static_cast<DynamicBlock*>(Dst)->GetType() == LeverSwitch)
				static_cast<DynamicBlock*>(Dst)->SetActivate();
		}
	}
}

void EditScene::Late_Update(float dt)
{
	ObjectMgr->Late_Update(dt);
}

#ifdef USE_IMGUI

void EditScene::ImGui_Main()
{
	ImGui::SetNextWindowPos({ 0.f, 0.f });
	ImGui::Begin("==== MineCraft Dungeon Map Editor ====", NULL, 0);
	ImGui_SaveLoad();
	ImGui_SetBlockType();
	ImGui_SetBlockUsage();
	ImGui_SetBlockInfo();
	ImGui::End();

	ImGui::SetNextWindowPos({ 0.f, 300.f });
	ImGui::Begin("==== Link IronCage & Lever ====");
	ImGui_LinkLever();
	ImGui::End();

	// ImGui::SetNextWindowPos({ 0.f, 600.f });
	// ImGui::Begin("==== Remove Dynamic Block ====");
	// ImGui_RemoveBlock();
	// ImGui::End();
}

void EditScene::ImGui_SaveLoad()
{
	ImGui::Text("Static Block Count : %d", staticBlocks.size());
	ImGui::Text("Chunk Count : %d", ChunkMgr->GetChunks().size());

	static char save[16]{}; ImGui::SetNextItemWidth(150);
	ImGui::InputText(" : SAVE ST", save, sizeof(save)); ImGui::SameLine();
	if (ImGui::Button("SV STAGE")) BlockMgr->SaveStage(save);

	static char load[16]{}; ImGui::SetNextItemWidth(150);
	ImGui::InputText(" : LOAD ST", load, sizeof(load)); ImGui::SameLine();
	if (ImGui::Button("LD STAGE"))
	{
		staticBlocks.clear();
		BlockMgr->LoadStage(load);
	}

	ImGui::SetNextItemWidth(100); ImGui::InputInt(" : WidthX /", &WidthX); ImGui::SameLine();
	ImGui::SetNextItemWidth(100); ImGui::InputInt(" : WidthZ", &WidthZ);
	ImGui::SetNextItemWidth(100); ImGui::InputInt(" : Height /", &Height); ImGui::SameLine();
	ImGui::SetNextItemWidth(100); ImGui::InputFloat(" : Scale", &Scale, 0.005f, 0.05f, "%.3f");

	if (ImGui::Button("IMD CREATE HEIGHTMAP"))
	{
		staticBlocks.clear();
		dynamicBlocks.clear();

		ObjectMgr->ClearList(ObjectType::StaticBlock);
		ObjectMgr->ClearList(ObjectType::DynamicBlock);

		TerrainCreater terrain;
		terrain.Free();

		CreateTerrain("heightMap");
		PlaceTerrainBlocks("heightMap");
	}
	ImGui::SameLine();
	if (ImGui::Button("CLEAR TERRAIN"))
	{
		staticBlocks.clear();
		dynamicBlocks.clear();

		ObjectMgr->ClearList(ObjectType::StaticBlock);
		ObjectMgr->ClearList(ObjectType::DynamicBlock);

		TerrainCreater terrain;
		terrain.Free();
	}

	auto chunks = ChunkMgr->GetChunks();
	for (auto& [key, chunk] : chunks)
		chunk->BuildChunkFace();
}

void EditScene::ImGui_SetBlockType()
{
	if (dynamicBlockType == DynamicBlockType::dBlockNone)
	{
		const char* staticBlockNames[] =
		{
			"NONE", "DIRT", "GRASS", "WOOD", "WOODPLANK",
			"STONE", "COBBLESTONE", "SMOOTH STONE", "STONE BRICK", "MOSSY STONE BRICK",
			"GLASS", "LEAF"
		};
		if (ImGui::Combo(" : Static Type", &selectedSBlockType, staticBlockNames, IM_ARRAYSIZE(staticBlockNames)))
		{
			staticBlockType = static_cast<StaticBlockType>(selectedSBlockType);
			staticBlockUsage = StaticBlockUsage::Basic; selectedSBlockUsage = 0;
			staticBlockAxis = StaticBlockAxis::sAY; selectedSBlockAxis = 1;
			staticBlockRot = StaticBlockRot::sZP; selectedSBlockRot = 0;
		}
	}

	if (staticBlockType == StaticBlockType::sBlockNone)
	{
		const char* dynamicBlockNames[] = { "NONE", "LEVER", "CHEST", "IRON CAGE", "BRIDGE"};
		if (ImGui::Combo(" : Dynamic Type", &selectedDBlockType, dynamicBlockNames, IM_ARRAYSIZE(dynamicBlockNames)))
			dynamicBlockType = static_cast<DynamicBlockType>(selectedDBlockType);
	}
}

void EditScene::ImGui_SetBlockUsage()
{
	if (staticBlockType == StaticBlockType::sBlockNone)
		return;

	std::vector<const char*> usageOptions;
	std::vector<StaticBlockUsage> usageEnums;
	switch (staticBlockType)
	{
	case StaticBlockType::Dirt: case StaticBlockType::GrassDirt: case StaticBlockType::Wood:
	case StaticBlockType::StoneBrick: case StaticBlockType::MossyStoneBrick:
		return;
	case StaticBlockType::WoodPlank: case StaticBlockType::Stone: case StaticBlockType::CobbleStone: case StaticBlockType::SmoothStone:
		usageOptions = { "BASIC", "HALF", "STAIR" };
		usageEnums = { StaticBlockUsage::Basic, StaticBlockUsage::Half, StaticBlockUsage::Stair };
		break;
	case StaticBlockType::Glass: case StaticBlockType::Leaf:
		staticBlockUsage = StaticBlockUsage::Alpha;
		return;
	}

	if (!usageOptions.empty())
	{
		if (ImGui::Combo(" : Usage", &selectedSBlockUsage, usageOptions.data(), static_cast<int>(usageOptions.size())))
		{
			staticBlockUsage = usageEnums[selectedSBlockUsage];
			staticBlockAxis = StaticBlockAxis::sAY; selectedSBlockAxis = 1;
			staticBlockRot = StaticBlockRot::sZP; selectedSBlockRot = 0;
		}
	}
}

void EditScene::ImGui_SetBlockInfo()
{
	if (staticBlockType != StaticBlockType::sBlockNone)
	{
		switch (staticBlockUsage)
		{
		case StaticBlockUsage::Basic:
		{
			const char* axisNames[] = { "X", "Y", "Z" };
			if (ImGui::Combo(" : Axis", &selectedSBlockAxis, axisNames, IM_ARRAYSIZE(axisNames)))
				staticBlockAxis = static_cast<StaticBlockAxis>(selectedSBlockAxis);
			break;
		}
		case StaticBlockUsage::Stair: case StaticBlockUsage::Fence:
		{
			const char* rotNames[] = { "+Z", "-Z", "+X", "-X" };
			if (ImGui::Combo(" : Rotation", &selectedSBlockRot, rotNames, IM_ARRAYSIZE(rotNames)))
				staticBlockRot = static_cast<StaticBlockRot>(selectedSBlockRot);
			break;
		}
		}
	}
	else
	{
		const char* dynamicRotNames[] = { "+X", "-X", "+Z", "-Z" };
		switch (dynamicBlockType)
		{
		case DynamicBlockType::BasicChest: case DynamicBlockType::IronCages:
			if (ImGui::Combo(" : Rotation", &selectedDBlockRot, dynamicRotNames, IM_ARRAYSIZE(dynamicRotNames)))
				dynamicBlockRot = static_cast<DynamicBlockRot>(selectedDBlockRot);
			if (dynamicBlockType == DynamicBlockType::IronCages)
				ImGui::InputInt(" : Count", &Count);
			break;
		}
	}
}

void EditScene::ImGui_LinkLever()
{
	static int selectedCageIndex = -1;
	static int selectedLeverIndex = -1;

	ImGui::Text("Iron Cages:");
	for (int i = 0; i < dynamicBlocks.size(); ++i)
	{
		if (dynamicBlocks[i].Type == DynamicBlockType::IronCages)
		{
			char buf[64];
			snprintf(buf, sizeof(buf), "Cage %d (Pos: %.1f, %.1f, %.1f)", i,
				dynamicBlocks[i].Pos.x, dynamicBlocks[i].Pos.y, dynamicBlocks[i].Pos.z);
			if (ImGui::Selectable(buf, selectedCageIndex == i))
				selectedCageIndex = i;
		}
	}

	if (selectedCageIndex != -1)
	{
		Object* cageObj = nullptr;
		_vec3 cagePos = dynamicBlocks[selectedCageIndex].Pos;

		for (auto& obj : ObjectMgr->GetObjectList(ObjectType::DynamicBlock))
		{
			if (obj->GetComponent<TransformComponent>()->GetPosition() == cagePos)
			{
				cageObj = obj;
				break;
			}
		}

		if (cageObj)
		{
			auto* cage = static_cast<DynamicBlock*>(cageObj);
			const auto& ids = cage->GetIDVec();

			ImGui::Separator();
			ImGui::Text("Linked Lever IDs:");
			if (!ids.empty())
			{
				for (int id : ids)
				{
					ImGui::BulletText("Lever ID: %d", id);
				}
			}
			else
			{
				ImGui::TextColored(ImVec4(1, 0.5f, 0.5f, 1), "None linked.");
			}
		}
	}

	ImGui::Separator();
	ImGui::Text("Levers:");
	std::vector<int> leverIDs;
	std::vector<std::string> leverLabels;
	for (int i = 0; i < dynamicBlocks.size(); ++i)
	{
		if (dynamicBlocks[i].Type == DynamicBlockType::LeverSwitch)
		{
			Object* leverObj = nullptr;
			for (auto& obj : ObjectMgr->GetObjectList(ObjectType::DynamicBlock))
			{
				if (obj->GetComponent<TransformComponent>()->GetPosition() == dynamicBlocks[i].Pos)
				{
					leverObj = obj;
					break;
				}
			}
			if (leverObj)
			{
				int leverID = static_cast<DynamicBlock*>(leverObj)->GetID();
				leverIDs.push_back(leverID);
				leverLabels.push_back("Lever ID " + std::to_string(leverID));
			}
		}
	}

	static int leverComboIdx = 0;
	if (!leverLabels.empty())
	{
		ImGui::Combo("Select Lever ID", &leverComboIdx, [](void* data, int idx, const char** out_text)
			{
				auto& labels = *static_cast<std::vector<std::string>*>(data);
				*out_text = labels[idx].c_str();
				return true;
			}, &leverLabels, leverLabels.size());
	}

	if (ImGui::Button("Link Lever to Cage") && selectedCageIndex != -1 && !leverIDs.empty())
	{
		int cageIndex = selectedCageIndex;
		int leverID = leverIDs[leverComboIdx];

		Object* cageObj = nullptr;
		for (auto& obj : ObjectMgr->GetObjectList(ObjectType::DynamicBlock))
		{
			if (obj->GetComponent<TransformComponent>()->GetPosition() == dynamicBlocks[cageIndex].Pos)
			{
				cageObj = obj;
				break;
			}
		}

		if (cageObj)
		{
			DynamicBlock* cage = static_cast<DynamicBlock*>(cageObj);
			auto idVec = cage->GetIDVec();
			if (std::find(idVec.begin(), idVec.end(), leverID) == idVec.end())
			{
				cage->AddID(leverID);
			}
		}
	}
}

void EditScene::ImGui_RemoveBlock()
{
	static int selectedDynamicIndex = -1;

	ImGui::Separator();
	ImGui::Text("Dynamic Blocks:");

	for (int i = 0; i < dynamicBlocks.size(); ++i)
	{
		const auto& block = dynamicBlocks[i];

		char buf[64];
		snprintf(buf, sizeof(buf), "[%d] %s (%.1f, %.1f, %.1f)", i,
			(block.Type == DynamicBlockType::LeverSwitch ? "Lever" :
				block.Type == DynamicBlockType::IronCages ? "Cage" : "Chest"),
			block.Pos.x, block.Pos.y, block.Pos.z);

		if (ImGui::Selectable(buf, selectedDynamicIndex == i))
		{
			selectedDynamicIndex = i;
			if (Object* cam = (ObjectMgr)->GetFrontObject(ObjectType::Camera))
			{
				_vec3 lookPos = block.Pos + _vec3(-5.f, 0.f, -5.f);
				static_cast<Camera*>(cam)->SetPosition(lookPos);
			}
		}
	}

	if (selectedDynamicIndex != -1)
	{
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1, 0.8f, 0.2f, 1), "Selected Block: %d", selectedDynamicIndex);
		if (ImGui::Button("Remove Dynamic Block"))
		{
			_vec3 posToDelete = dynamicBlocks[selectedDynamicIndex].Pos;
			ObjectMgr->RemoveObject(ObjectType::DynamicBlock, posToDelete);
			dynamicBlocks.erase(dynamicBlocks.begin() + selectedDynamicIndex);
			selectedDynamicIndex = -1;
		}
	}
}

#endif

void EditScene::MakePickingRay(_vec3& outRayOrigin, _vec3& outRayDir)
{
	LPDIRECT3DDEVICE9 device = GraphicDevice::GetInstance()->GetDevice();
	auto cam = ObjectMgr->GetFrontObject(ObjectType::Camera)->GetComponent<CameraComponent>();

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(EngineCore::GetInstance()->GetWindowHandle(), &pt);

	D3DVIEWPORT9 vp;
	device->GetViewport(&vp);

	_vec3 vMouseNear, vMouseFar;
	vMouseNear.x = (float)pt.x / (float)vp.Width * 2.f - 1.f;
	vMouseNear.y = 1.f - (float)pt.y / (float)vp.Height * 2.f;
	vMouseNear.z = 0.f;

	vMouseFar = vMouseNear;
	vMouseFar.z = 1.f;

	_matrix invProj = cam->GetProjMatrix();
	D3DXMatrixInverse(&invProj, nullptr, &invProj);
	D3DXVec3TransformCoord(&vMouseNear, &vMouseNear, &invProj);
	D3DXVec3TransformCoord(&vMouseFar, &vMouseFar, &invProj);

	_matrix invView = cam->GetViewMatrix();
	D3DXMatrixInverse(&invView, nullptr, &invView);
	D3DXVec3TransformCoord(&vMouseNear, &vMouseNear, &invView);
	D3DXVec3TransformCoord(&vMouseFar, &vMouseFar, &invView);

	outRayOrigin = vMouseNear;
	outRayDir = vMouseFar - vMouseNear;
	D3DXVec3Normalize(&outRayDir, &outRayDir);
}

void EditScene::OnLeftClick(_vec3& rayOrigin, _vec3& rayDir)
{
	bool found(false);
	float closestDist(FLT_MAX);

	_vec3 closestHitPoint;
	_vec3 selectedBlockPos;

	for (const auto& block : staticBlocks)
	{
		float dist(0.f);
		_vec3 min = block.Pos - _vec3(1, 1, 1);
		_vec3 max = block.Pos + _vec3(1, 1, 1);

		if (RayIntersectsAABB(rayOrigin, rayDir, min, max, dist))
		{
			if (dist < closestDist)
			{
				closestDist = dist;
				selectedBlockPos = block.Pos;
				closestHitPoint = rayOrigin + rayDir * dist;
				found = true;
			}
		}
	}	

	if (!found) return;

	_vec3 normal = GetHitNormal(closestHitPoint, selectedBlockPos - _vec3(1, 1, 1), selectedBlockPos + _vec3(1, 1, 1));
	_vec3 newBlockPos = selectedBlockPos + normal * 2.0f;

	Place(newBlockPos);
}

void EditScene::OnRightClick(_vec3& rayOrigin, _vec3& rayDir)
{
	bool found(false);
	int targetIndex(-1);
	float closestDist(FLT_MAX);

	for (int i = 0; i < staticBlocks.size(); ++i)
	{
		const auto& block = staticBlocks[i];
		_vec3 min = block.Pos - _vec3(1.f, 1.f, 1.f);
		_vec3 max = block.Pos + _vec3(1.f, 1.f, 1.f);

		float dist(0.f);
		if (RayIntersectsAABB(rayOrigin, rayDir, min, max, dist))
		{
			if (dist < closestDist)
			{
				closestDist = dist;
				targetIndex = i;
				found = true;
			}
		}
	}

	if (found)
	{
		ObjectMgr->RemoveObject(ObjectType::StaticBlock, (staticBlocks.begin() + targetIndex)->Pos);
		staticBlocks.erase(staticBlocks.begin() + targetIndex);
		return;
	}
}

void EditScene::CreateTerrain(const std::string& filename)
{
	TerrainCreater terrain;
	terrain.CreateHeightmap(WidthX, WidthZ, Scale);
	terrain.SaveHeightmapAsImage(filename);
}

void EditScene::PlaceTerrainBlocks(const std::string& filename)
{
	TerrainCreater terrain;
	if (!terrain.LoadHeightmapFromImage(filename)) return;

	terrain.CreateBlockTerrain(WidthX, WidthZ, Height);

	for (const auto& block : terrain.GetBlocks())
	{
		_vec3 position = block.Pos * 2.f;
		Place(position);
	}
}

bool EditScene::RayIntersectsAABB(const _vec3& rayOrigin, const _vec3& rayDir, const _vec3& boxMin, const _vec3& boxMax, float& outDistance)
{
	float tMin = 0.f;
	float tMax = FLT_MAX;

	for (int i = 0; i < 3; ++i)
	{
		if (fabs(rayDir[i]) < 1e-6)
		{
			if (rayOrigin[i] < boxMin[i] || rayOrigin[i] > boxMax[i])
				return false;
		}
		else
		{
			float t1 = (boxMin[i] - rayOrigin[i]) / rayDir[i];
			float t2 = (boxMax[i] - rayOrigin[i]) / rayDir[i];

			if (t1 > t2) swap(t1, t2);

			tMin = max(tMin, t1);
			tMax = min(tMax, t2);

			if (tMin > tMax)
				return false;
		}
	}

	outDistance = tMin;
	return true;
}

_vec3 EditScene::GetHitNormal(const _vec3& hitPoint, const _vec3& boxMin, const _vec3& boxMax)
{
	if (fabs(hitPoint.x - boxMin.x) < 0.01f)
	{
		dynamicBlockCol = DynamicBlockCol::dXM;
		return _vec3(-1, 0, 0);
	}
	if (fabs(hitPoint.x - boxMax.x) < 0.01f)
	{
		dynamicBlockCol = DynamicBlockCol::dXP;
		return _vec3(1, 0, 0);
	}

	if (fabs(hitPoint.y - boxMin.y) < 0.01f)
	{
		dynamicBlockCol = DynamicBlockCol::dAEnd;
		return _vec3(0, -1, 0);
	}
	if (fabs(hitPoint.y - boxMax.y) < 0.01f)
	{
		dynamicBlockCol = DynamicBlockCol::dYP;
		return _vec3(0, 1, 0);
	}

	if (fabs(hitPoint.z - boxMin.z) < 0.01f)
	{
		dynamicBlockCol = DynamicBlockCol::dZM;
		return _vec3(0, 0, -1);
	}
	if (fabs(hitPoint.z - boxMax.z) < 0.01f)
	{
		dynamicBlockCol = DynamicBlockCol::dZP;
		return _vec3(0, 0, 1);
	}
	
	return _vec3(0, 0, 0);
}

void EditScene::Place(_vec3& position)
{
	for (const auto& block : staticBlocks) if (block.Pos == position) return;
	for (const auto& block : dynamicBlocks) if (block.Pos == position) return;
	
	if (staticBlockType != StaticBlockType::sBlockNone)
	{
		auto newBlockObj = StaticBlock::Create(ObjectMgr, ObjectType::StaticBlock, staticBlockType, staticBlockAxis, staticBlockRot, staticBlockUsage);
		if (!newBlockObj) return;
		position += newBlockObj->GetComponent<TransformComponent>()->GetPosition();
		newBlockObj->GetComponent<TransformComponent>()->SetPosition(position);
		ObjectMgr->AddObject(ObjectType::StaticBlock, newBlockObj);
		
		int chunkX = int(position.x / CHUNK_SIZE);
		int chunkY = int(position.z / CHUNK_SIZE);

		Chunk* chunk = ChunkMgr->CreateChunk(chunkX, chunkY);
		chunk->AddBlock(position, staticBlockType, staticBlockAxis, staticBlockRot, staticBlockUsage);
		chunk->BuildChunkFace();

		staticBlocks.push_back({ position, staticBlockType, staticBlockAxis, staticBlockRot, staticBlockUsage });
	}
	else if (dynamicBlockType != DynamicBlockType::dBlockNone)
	{
		Object* newBlockObj = DynamicBlock::Create(ObjectMgr, ObjectType::DynamicBlock, dynamicBlockType, dynamicBlockCol,dynamicBlockRot, Count);
		if (!newBlockObj) return;
		newBlockObj->GetComponent<TransformComponent>()->SetPosition(position);
		ObjectMgr->AddObject(ObjectType::DynamicBlock, newBlockObj);
	
		dynamicBlocks.push_back({ position, dynamicBlockType, dynamicBlockCol, dynamicBlockRot });
	}
}

void EditScene::Free()
{
	Safe_Release(ObjectMgr);
	Safe_Release(CollisionSys);
	Safe_Release(CameraMgr);
	Safe_Release(BlockMgr);
	Safe_Release(ChunkMgr);
	staticBlocks.clear();
	staticBlocks.shrink_to_fit();
	Scene::Free();
}