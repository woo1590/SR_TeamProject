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
#include "Chunk.h"
#include "Material.h"
#include "TerrainCreater.h"
#include "GraphicDevice.h"

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
	ObjectMgr = ObjectManager::Create(this);
	CameraMgr = CameraManager::Create(this);
	CollisionSys = CollisionSystem::Create(this);

	Terrain = new TerrainCreater;
	BlockMgr = BlockManager::Create(this);
	ChunkMgr = ChunkManager::Create(this);
	
	BlockMgr->LoadTexture(); // 이미지 불러오기

	// 지형 생성 위한 블럭 1개 추가
	ChunkMgr->CreateChunk(0, 0);
	ChunkMgr->GetChunk(0, 0)->AddBlock({ 1, 1, 1 }, StaticBlockType::Dirt, StaticBlockAxis::sAY, StaticBlockRot::sZP, StaticBlockUsage::Basic);
	ChunkMgr->GetChunk(0, 0)->BuildChunkFace();
	
	SB baseBlock{ {1, 1, 1}, StaticBlockType::Dirt, StaticBlockAxis::sAY, StaticBlockRot::sZP, StaticBlockUsage::Basic };
	staticBlocks.push_back(baseBlock);

	// 카메라 추가 및 설정
	auto cam = Camera::Create(ObjectMgr, ObjectType::Camera);
	ObjectMgr->AddObject(ObjectType::Camera, cam);
	CameraMgr->AddCamera(L"ToolCam", cam);
	CameraMgr->SetMainCamera(L"ToolCam");
}

void EditScene::Update(float dt)
{
	ObjectMgr->Update(dt);

	// 좌&우클릭에 따른 블럭 생성&제거
	_vec3 rayOrigin, rayDir;
	ImGuiIO& io = ImGui::GetIO();
	auto Input = EngineCore::GetInstance()->GetInputSystem();

	if (!io.WantCaptureMouse)
	{
		if (isDown)
		{
			if (Input->IsKeyDown(LBUTTON))
			{
				MakePickingRay(rayOrigin, rayDir);
				OnLeftClick(rayOrigin, rayDir);
			}
			if (Input->IsKeyDown(RBUTTON))
			{
				MakePickingRay(rayOrigin, rayDir);
				OnRightClick(rayOrigin, rayDir);
			}
		}
		else
		{
			if (Input->IsKeyPressed(LBUTTON))
			{
				MakePickingRay(rayOrigin, rayDir);
				OnLeftClick(rayOrigin, rayDir);
			}
			if (Input->IsKeyPressed(RBUTTON))
			{
				MakePickingRay(rayOrigin, rayDir);
				OnRightClick(rayOrigin, rayDir);
			}
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
	ImGui_Info();
	ImGui_SaveLoad();
	ImGui_SetBlockType();
	ImGui_SetBlockUsage();
	ImGui_SetBlockInfo();
	ImGui::End();

	ImGui::SetNextWindowPos({ 0.f, 300.f });
	ImGui::Begin("==== Link IronCage & Lever ====");
	ImGui_LinkLever();
	ImGui::End();
}

void EditScene::ImGui_Info()
{
	static bool isChecked(false);
	if (ImGui::Checkbox(" : MOUSE DOWN", &isChecked)) isDown = isChecked;

	ImGui::Text("Chunk Count : %d", ChunkMgr->GetChunks().size());
	ImGui::Text("Static Block Count : %d", staticBlocks.size());
}

void EditScene::ImGui_SaveLoad()
{
	// 청크 저장하기
	static char save[16]{}; ImGui::SetNextItemWidth(150);
	ImGui::InputText(" : SAVE", save, sizeof(save)); ImGui::SameLine();
	if (ImGui::Button("SAVE CHUNK")) BlockMgr->SaveChunk(save);

	// 청크 불러오기
	static char load[16]{}; ImGui::SetNextItemWidth(150);
	ImGui::InputText(" : LOAD", load, sizeof(load)); ImGui::SameLine();
	if (ImGui::Button("LOAD CHUNK"))
	{
		Terrain->Free();
		staticBlocks.clear();
		ChunkMgr->ClearAllChunks();

		dynamicBlocks.clear();
		ObjectMgr->ClearList(ObjectType::DynamicBlock);

		BlockMgr->LoadChunk(load);

		for (auto& chunk : ChunkMgr->GetChunks())
		{
			for (int y = 0; y < CHUNK_HEIGHT; ++y)
			for (int z = 0; z < CHUNK_SIZE; ++z)
			for (int x = 0; x < CHUNK_SIZE; ++x)
			{
				auto bInfo = chunk.second->GetBlock(x, y, z);
				if (bInfo.Type == StaticBlockType::Air) continue;

				staticBlocks.push_back({ bInfo.Pos, bInfo.Type, bInfo.Axis, bInfo.Rot, bInfo.Usage });
			}
		}
	}

	// 생성할 지형 넓이 지정
	ImGui::SetNextItemWidth(100); ImGui::InputInt(" : WidthX /", &WidthX); ImGui::SameLine();
	ImGui::SetNextItemWidth(100); ImGui::InputInt(" : WidthZ", &WidthZ);

	// 생성할 지형 높이와 굴곡도 지정
	ImGui::SetNextItemWidth(100); ImGui::InputInt(" : Height /", &Height); ImGui::SameLine();
	ImGui::SetNextItemWidth(100); ImGui::InputFloat(" : Scale", &Scale, 0.005f, 0.05f, "%.3f");

	if (ImGui::Button("IMD CREATE HEIGHTMAP"))
	{
		Terrain->Free();
		staticBlocks.clear();
		ChunkMgr->ClearAllChunks();

		CreateTerrain("heightMap");
		PlaceTerrainBlocks("heightMap");

		selectedSBlockType = 0;
		dynamicBlocks.clear();
		ObjectMgr->ClearList(ObjectType::DynamicBlock);
	}
	ImGui::SameLine();
	if (ImGui::Button("CLEAR TERRAIN"))
	{
		Terrain->Free();
		staticBlocks.clear();
		ChunkMgr->ClearAllChunks();

		dynamicBlocks.clear();
		ObjectMgr->ClearList(ObjectType::DynamicBlock);
	}
}

void EditScene::ImGui_SetBlockUsage()
{
	if (staticBlockType == StaticBlockType::Air) return;

	std::vector<const char*> usageOptions;
	std::vector<StaticBlockUsage> usageEnums;

	// 블럭 모양 결정
	switch (staticBlockType)
	{
	case StaticBlockType::Dirt: case StaticBlockType::GrassDirt: case StaticBlockType::Wood:
	case StaticBlockType::StoneBrick: case StaticBlockType::MossyStoneBrick:
		staticBlockUsage = StaticBlockUsage::Basic;
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

void EditScene::ImGui_SetBlockType()
{
	if (dynamicBlockType == DynamicBlockType::dBlockNone)
	{
		// 청크 종류 선택
		const char* staticBlockNames[] =
		{
			"NONE", "DIRT", "GRASS", "WOOD", "WOODPLANK",
			"STONE", "COBBLESTONE", "SMOOTH STONE", "STONE BRICK", "MOSSY STONE BRICK",
			"GLASS", "LEAF"
		};

		// 선택할 때마다, 다른 값들 초기화
		if (ImGui::Combo(" : Static Type", &selectedSBlockType, staticBlockNames, IM_ARRAYSIZE(staticBlockNames)))
		{
			staticBlockType = static_cast<StaticBlockType>(selectedSBlockType);
			selectedSBlockUsage = 0; staticBlockUsage = StaticBlockUsage::Basic;
			selectedSBlockAxis = 1; staticBlockAxis = StaticBlockAxis::sAY;
			selectedSBlockRot = 0; staticBlockRot = StaticBlockRot::sZP;
		}
	}

	if (staticBlockType == StaticBlockType::Air)
	{
		const char* dynamicBlockNames[] = { "NONE", "LEVER", "CHEST", "IRON CAGE", "BRIDGE"};

		if (ImGui::Combo(" : Dynamic Type", &selectedDBlockType, dynamicBlockNames, IM_ARRAYSIZE(dynamicBlockNames)))
			dynamicBlockType = static_cast<DynamicBlockType>(selectedDBlockType);
	}
}

void EditScene::ImGui_SetBlockInfo()
{
	if (staticBlockType != StaticBlockType::Air)
	{
		const char* axisNames[] = { "X", "Y", "Z" };
		const char* rotNames[] = { "+Z", "-Z", "+X", "-X" };

		// 블럭 방향 및 회전 지정
		switch (staticBlockUsage)
		{
		case StaticBlockUsage::Basic:
			if (ImGui::Combo(" : Axis", &selectedSBlockAxis, axisNames, IM_ARRAYSIZE(axisNames)))
				staticBlockAxis = static_cast<StaticBlockAxis>(selectedSBlockAxis);
			break;

		case StaticBlockUsage::Stair: case StaticBlockUsage::Fence:
			if (ImGui::Combo(" : Rotation", &selectedSBlockRot, rotNames, IM_ARRAYSIZE(rotNames)))
				staticBlockRot = static_cast<StaticBlockRot>(selectedSBlockRot);
			break;
		}
	}
	else
	{
		const char* dynamicRotNames[] = { "+X", "-X", "+Z", "-Z" };

		// 블럭 방향 및 개수 지정
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

	// 철창 오브젝트 위치 띄우기
	ImGui::Text("IRON CAGE : ");
	for (int i = 0; i < dynamicBlocks.size(); ++i)
	{
		char buf[64];

		if (dynamicBlocks[i].Type == DynamicBlockType::IronCages)
		{
			snprintf(buf, sizeof(buf), "CAGE %d : ( POSITION : %.1f, %.1f, %.1f )",
				i, dynamicBlocks[i].Pos.x, dynamicBlocks[i].Pos.y, dynamicBlocks[i].Pos.z);

			if (ImGui::Selectable(buf, selectedCageIndex == i))
				selectedCageIndex = i;
		}
	}

	// 철창에 링크된 레버 ID 띄우기
	if (selectedCageIndex != -1)
	{
		Object* cageObj(nullptr);
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
			ImGui::Text("LINKED LEVER ID : ");

			if (!ids.empty()) for (int id : ids) ImGui::BulletText("LEVER ID : %d", id);
			else ImGui::TextColored(ImVec4(1, 0.5f, 0.5f, 1), "NONE LINKED");
		}
	}

	ImGui::Separator();

	ImGui::Text("LEVER : ");
	std::vector<int> leverIDs;
	std::vector<std::string> leverLabels;

	// 레버 오브젝트 ID 띄우기
	for (int i = 0; i < dynamicBlocks.size(); ++i)
	{
		if (dynamicBlocks[i].Type == DynamicBlockType::LeverSwitch)
		{
			Object* leverObj(nullptr);

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
				leverLabels.push_back("LEVER ID " + std::to_string(leverID));
			}
		}
	}

	static int leverComboIdx(0);

	// 레버와 케이지 링크하기
	if (!leverLabels.empty())
	{
		ImGui::Combo("SELECT LEVER ID", &leverComboIdx, [](void* data, int idx, const char** out_text)
			{
				auto& labels = *static_cast<std::vector<std::string>*>(data);
				*out_text = labels[idx].c_str();
				return true;
			}, &leverLabels, leverLabels.size());
	}

	if (ImGui::Button("LINK LEVER TO CAGE") && selectedCageIndex != -1 && !leverIDs.empty())
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

			if (std::find(idVec.begin(), idVec.end(), leverID) == idVec.end()) cage->AddID(leverID);
		}
	}
}
#endif

void EditScene::CreateTerrain(const std::string& filename)
{
	Terrain->CreateHeightmap(WidthX, WidthZ, Scale);
	Terrain->SaveHeightmapAsImage(filename);
}

void EditScene::PlaceTerrainBlocks(const std::string& filename)
{
	CreateTer = true;
	ChunkMgr->ClearAllChunks();

	if (!Terrain->LoadHeightmapFromImage(filename)) return;
	Terrain->CreateBlockTerrain(WidthX, WidthZ, Height);

	for (const auto& block : Terrain->GetBlocks())
	{
		_vec3 position = block.Pos;
		staticBlockUsage = block.Usage;
		staticBlockType = block.Type;
		staticBlockAxis = block.Axis;
		staticBlockRot = block.Rot;

		Place(position);
	}

	CreateTer = false;
	for (auto& iter : ChunkMgr->GetChunks())
		iter.second->BuildChunkFace();
}

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

bool EditScene::RayIntersectsAABB(const _vec3& rayOrigin, const _vec3& rayDir, const _vec3& boxMin, const _vec3& boxMax, float& outDistance)
{
	float tMin(0.f);
	float tMax(FLT_MAX);

	for (int i = 0; i < 3; ++i)
	{
		if (fabs(rayDir[i]) < 1e-6)
		{
			if (rayOrigin[i] < boxMin[i] || rayOrigin[i] > boxMax[i]) return false;
		}
		else
		{
			float t1 = (boxMin[i] - rayOrigin[i]) / rayDir[i];
			float t2 = (boxMax[i] - rayOrigin[i]) / rayDir[i];

			if (t1 > t2) swap(t1, t2);

			tMin = max(tMin, t1);
			tMax = min(tMax, t2);

			if (tMin > tMax) return false;
		}
	}

	outDistance = tMin;
	return true;
}

void EditScene::OnLeftClick(_vec3& rayOrigin, _vec3& rayDir)
{
	bool found(false);				// 광선에 충돌한 블럭 유무
	float closestDist(FLT_MAX);		// 가장 가까운 거리
	_vec3 closestHitPoint;			// 광선이 맞은 곳 위치
	_vec3 selectedBlockPos;			// 선택된 블럭 위치

	for (const auto& block : staticBlocks)
	{
		float dist(0.f);
		_vec3 min = block.Pos - _vec3(1, 1, 1);	// 블럭 최하 점
		_vec3 max = block.Pos + _vec3(1, 1, 1);	// 블럭 최상 점

		// 현재 충돌한 블럭이 이전 위치보다 가까울 시, 거리 및 위치 등 갱신
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

	if (!found) return;	// 못 찾았으면 바로 리턴

	// 현재 충돌한 블럭의 중심점에서 크기의 반만큼 빼서 다음 블럭 위치로 향하는 법선 구하기
	_vec3 normal = GetHitNormal(closestHitPoint, selectedBlockPos - _vec3(1, 1, 1), selectedBlockPos + _vec3(1, 1, 1));

	// 구한 법선에 블럭 크기를 곱하여 다음 블럭 위치 구하기
	_vec3 newBlockPos = selectedBlockPos + normal * BLOCK_SIZE;

	Place(newBlockPos);
}

void EditScene::OnRightClick(_vec3& rayOrigin, _vec3& rayDir)
{
	bool found(false);				// 광선에 충돌한 블럭 유무
	float closestDist(FLT_MAX);		// 가장 가까운 거리
	_vec3 closestHitPoint;			// 광선이 맞은 곳 위치
	_vec3 selectedBlockPos;			// 선택된 블럭 위치

	for (const auto& block : staticBlocks)
	{
		float dist(0.f);
		_vec3 min = block.Pos - _vec3(1, 1, 1);	// 블럭 최하 점
		_vec3 max = block.Pos + _vec3(1, 1, 1);	// 블럭 최상 점

		// 현재 충돌한 블럭이 이전 위치보다 가까울 시, 거리 및 위치 등 갱신
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

	if (found)
	{
		// 충돌한 블럭의 인덱스로 구한 위치로 청크 좌표 구하기
		auto [chunkX, chunkZ] = GetChunkCoordFromWorldPos(selectedBlockPos);
		auto targetChunk = ChunkMgr->GetChunk(chunkX, chunkZ);

		for (auto iter = staticBlocks.begin(); iter != staticBlocks.end(); )
		{
			if (iter->Pos == selectedBlockPos)
			{
				iter = staticBlocks.erase(iter);
				break;
			}
			else
			{
				++iter;
			}
		}

		if (targetChunk)
		{
			_vec3 localPos = GetLocalCoordInChunk(selectedBlockPos, chunkX, chunkZ);

			int lx = static_cast<int>(localPos.x);
			int ly = static_cast<int>(localPos.y);
			int lz = static_cast<int>(localPos.z);

			if (lx >= 0 && lx < CHUNK_SIZE && ly >= 0 && ly < CHUNK_HEIGHT && lz >= 0 && lz < CHUNK_SIZE)
			{
				targetChunk->SetBlockAir(lx, ly, lz);
				targetChunk->BuildChunkFace();
			}
		}
	}
}

_vec3 EditScene::GetHitNormal(const _vec3& hitPoint, const _vec3& boxMin, const _vec3& boxMax)
{
	// 매개변수 : 충돌한 위치, 충돌한 블럭의 최소 점, 최상 점

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
	// 새로 설치하려는 블럭의 위치에 이미 다른 블럭이 존재하면 바로 리턴
	for (const auto& block : staticBlocks) if (block.Pos == position) return;
	for (const auto& block : dynamicBlocks) if (block.Pos == position) return;
	
	if (staticBlockType != StaticBlockType::Air)
	{
		// ================ 청크 생성 ================
		int chunkX = static_cast<int>(floorf(position.x / CHUNK_SIZE));
		int chunkZ = static_cast<int>(floorf(position.z / CHUNK_SIZE));

		ChunkMgr->CreateChunk(chunkX, chunkZ);
		ChunkMgr->GetChunk(chunkX, chunkZ)->AddBlock(position, staticBlockType, staticBlockAxis, staticBlockRot, staticBlockUsage);
		if (!CreateTer) ChunkMgr->GetChunk(chunkX, chunkZ)->BuildChunkFace();

		// ================ 벡터에 블럭 정보 삽입 ================
		staticBlocks.push_back({ position, staticBlockType, staticBlockAxis, staticBlockRot, staticBlockUsage });
	}
	else if (dynamicBlockType != DynamicBlockType::dBlockNone)
	{
		auto newBlockObj = DynamicBlock::Create(ObjectMgr,
			ObjectType::DynamicBlock, dynamicBlockType, dynamicBlockCol,dynamicBlockRot, Count);
		if (!newBlockObj) return;

		newBlockObj->GetComponent<TransformComponent>()->SetPosition(position);
		ObjectMgr->AddObject(ObjectType::DynamicBlock, newBlockObj);
	
		dynamicBlocks.push_back({ position, dynamicBlockType, dynamicBlockCol, dynamicBlockRot });
	}
}

std::pair<int, int> EditScene::GetChunkCoordFromWorldPos(const _vec3& pos)
{
	int chunkX = static_cast<int>(floor(pos.x / CHUNK_SIZE));
	int chunkZ = static_cast<int>(floor(pos.z / CHUNK_SIZE));
	return { chunkX, chunkZ };
}

_vec3 EditScene::GetLocalCoordInChunk(const _vec3& pos, int chunkX, int chunkZ)
{
	int localX = static_cast<int>(pos.x / BLOCK_SIZE) % CHUNK_SIZE;
	int localY = static_cast<int>(pos.y / BLOCK_SIZE);
	int localZ = static_cast<int>(pos.z / BLOCK_SIZE) % CHUNK_SIZE;
	return { (float)localX, (float)localY, (float)localZ };
}

void EditScene::Free()
{
	Safe_Release(ChunkMgr);
	Safe_Release(ObjectMgr);
	Safe_Release(CollisionSys);
	Safe_Release(CameraMgr);

	Safe_Release(BlockMgr);
	Safe_Release(Terrain);

	staticBlocks.clear();
	staticBlocks.shrink_to_fit();

	dynamicBlocks.clear();
	dynamicBlocks.shrink_to_fit();

	Scene::Free();
}