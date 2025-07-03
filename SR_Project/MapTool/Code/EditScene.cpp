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

//object
#include "Lever.h"
#include "Chest.h"
#include "Camera.h"
#include "StaticBlock.h"

//component
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"

#include "CubeMesh.h"
#include "Material.h"
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
	EngineCore::GetInstance()->GetImGuiManager()->RegisterWindow(L"MapToolTest", [this]() {this->ImGuiTest();});
	CollisionSys = CollisionSystem::Create(this);

	auto cube = CubeMesh::Create();
	auto resource = EngineCore::GetInstance()->GetResourceManager();
	
	resource->LoadMesh(L"Cube_Mesh", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/DirtBlock.dds", L"DirtBlock", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"DirtBlock", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/GrassBlock.dds", L"GrassBlock", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"GrassBlock", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/WoodBlock.dds", L"WoodBlock", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"WoodBlock", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/WoodPlank.dds", L"WoodPlank", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"WoodPlank", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/Stone.dds", L"Stone", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"Stone", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/CobbleStone.dds", L"CobbleStone", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"CobbleStone", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/Lever.dds", L"Lever", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"Lever", cube);
	
	resource->LoadTexture(L"../Resource/Texture/Block/ChestDown.dds", L"ChestDown", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"ChestDown", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/ChestUp.dds", L"ChestUp", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"ChestUp", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/ChestLock.dds", L"ChestLock", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"ChestLock", cube);

	auto dirtBlockMtrl = Material::Create();
	dirtBlockMtrl->SetTexture(L"DirtBlock");
	resource->LoadMaterial(L"DirtBlock_Mtrl", dirtBlockMtrl);

	auto grassBlockMtrl = Material::Create();
	grassBlockMtrl->SetTexture(L"GrassBlock");
	resource->LoadMaterial(L"GrassBlock_Mtrl", grassBlockMtrl);

	auto woodBlockMtrl = Material::Create();
	woodBlockMtrl->SetTexture(L"WoodBlock");
	resource->LoadMaterial(L"WoodBlock_Mtrl", woodBlockMtrl);

	auto woodPlankMtrl = Material::Create();
	woodPlankMtrl->SetTexture(L"WoodPlank");
	resource->LoadMaterial(L"WoodPlank_Mtrl", woodPlankMtrl);

	auto stoneMtrl = Material::Create();
	stoneMtrl->SetTexture(L"Stone");
	resource->LoadMaterial(L"Stone_Mtrl", stoneMtrl);

	auto cobbleStoneMtrl = Material::Create();
	cobbleStoneMtrl->SetTexture(L"CobbleStone");
	resource->LoadMaterial(L"CobbleStone_Mtrl", cobbleStoneMtrl);

	auto leverMtrl = Material::Create();
	leverMtrl->SetTexture(L"Lever");
	resource->LoadMaterial(L"Lever_Mtrl", leverMtrl);

	auto chestUpMtrl = Material::Create();
	chestUpMtrl->SetTexture(L"ChestUp");
	resource->LoadMaterial(L"ChestUp_Mtrl", chestUpMtrl);

	auto chestDownMtrl = Material::Create();
	chestDownMtrl->SetTexture(L"ChestDown");
	resource->LoadMaterial(L"ChestDown_Mtrl", chestDownMtrl);

	auto chestLockMtrl = Material::Create();
	chestLockMtrl->SetTexture(L"ChestLock");
	resource->LoadMaterial(L"ChestLock_Mtrl", chestLockMtrl);


	SB baseBlock{ {0, 0, 0}, staticBlockType, staticBlockDir };
	staticBlocks.push_back(baseBlock);

	ObjectMgr = ObjectManager::Create(this);
	auto cam = Camera::Create(ObjectMgr, ObjectType::Camera);
	ObjectMgr->AddObject(ObjectType::Camera, cam);
	ObjectMgr->AddObject(ObjectType::StaticBlock, StaticBlock::Create(ObjectMgr, ObjectType::StaticBlock, staticBlockType, staticBlockDir));

	CameraMgr = CameraManager::Create(this);
	CameraMgr->AddCamera(L"ToolCam", cam);
	CameraMgr->SetMainCamera(L"ToolCam");
}

void EditScene::Update(float dt)
{
	ObjectMgr->Update(dt);

	wchar_t title[128];
	swprintf_s(title, L"staticBlocks : %d", (int)staticBlocks.size());
	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), title);

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
}

void EditScene::Late_Update(float dt)
{
	ObjectMgr->Late_Update(dt);
}

void EditScene::Unload()
{
}

void EditScene::ImGuiTest()
{
	ImGui::SetNextWindowPos({ 0.f, 0.f });
	ImGui::Begin("==== MineCraft Dungeon Map Editor ====", NULL, 0);

	static char save[64]{};
	ImGui::InputText("<- Save Stage Name", save, sizeof(save));
	if (ImGui::Button("SAVE")) SaveStage(save);

	static char load[64]{};
	ImGui::InputText("<- Load Stage Name", load, sizeof(load));
	if (ImGui::Button("LOAD")) LoadStage(load);

	const char* staticBlockNames[] = { "Dirt", "GrassDirt", "Wood", "WoodPlank", "Stone", "CobbleStone", "None"};
	if (ImGui::Combo("<- Static Type", &selectedSBlockType, staticBlockNames, IM_ARRAYSIZE(staticBlockNames)))
		staticBlockType = static_cast<StaticBlockType>(selectedSBlockType);

	if (staticBlockType == StaticBlockType::SBlockNone)
	{
		const char* dynamicBlockNames[] = { "Lever", "Chest", "None" };
		if (ImGui::Combo("<- Dynamic Type", &selectedDBlockType, dynamicBlockNames, IM_ARRAYSIZE(dynamicBlockNames)))
			dynamicBlockType = static_cast<DynamicBlockType>(selectedDBlockType);
	}

	ImGui::End();

	ImGui::SetNextWindowPos({ 0.f, 300.f });
	ImGui::SetNextWindowSize({ 200.f, 200.f });
	ImGui::Begin("==== BLOCK IMG ====");

	const char* blockDirNames[] = { "X", "Y", "Z" };
	if (ImGui::Combo("<- Dir", &selectedSBlockDir, blockDirNames, IM_ARRAYSIZE(blockDirNames)))
		staticBlockDir = static_cast<StaticBlockDir>(selectedSBlockDir);

	ImGui::End();
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

	if (!found) return;

	ObjectMgr->RemoveObject(ObjectType::StaticBlock, (staticBlocks.begin() + targetIndex)->Pos);
	staticBlocks.erase(staticBlocks.begin() + targetIndex);
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
		dynamicBlockDir = DynamicBlockDir::XM;
		return _vec3(-1, 0, 0);
	}
	if (fabs(hitPoint.x - boxMax.x) < 0.01f)
	{
		dynamicBlockDir = DynamicBlockDir::XP;
		return _vec3(1, 0, 0);
	}

	if (fabs(hitPoint.y - boxMin.y) < 0.01f)
	{
		dynamicBlockDir = DynamicBlockDir::DBEnd;
		return _vec3(0, -1, 0);
	}
	if (fabs(hitPoint.y - boxMax.y) < 0.01f)
	{
		dynamicBlockDir = DynamicBlockDir::DBEnd;
		return _vec3(0, 1, 0);
	}

	if (fabs(hitPoint.z - boxMin.z) < 0.01f)
	{
		dynamicBlockDir = DynamicBlockDir::ZM;
		return _vec3(0, 0, -1);
	}
	if (fabs(hitPoint.z - boxMax.z) < 0.01f)
	{
		dynamicBlockDir = DynamicBlockDir::ZP;
		return _vec3(0, 0, 1);
	}
	
	return _vec3(0, 0, 0);
}

void EditScene::Place(const _vec3& position)
{
	for (const auto& block : staticBlocks)
		if (block.Pos == position && staticBlockType != StaticBlockType::SBlockNone) return;
	for (const auto& block : dynamicBlocks)
		if (block.Pos == position) return;

	if (staticBlockType != StaticBlockType::SBlockNone)
	{
		staticBlocks.push_back({ position, staticBlockType, staticBlockDir });

		auto newBlockObj = StaticBlock::Create(ObjectMgr, ObjectType::StaticBlock, staticBlockType, staticBlockDir);
		newBlockObj->GetComponent<TransformComponent>()->SetPosition(position);
		ObjectMgr->AddObject(ObjectType::StaticBlock, newBlockObj);
	}
	else
	{
		if (dynamicBlockDir == DynamicBlockDir::DBEnd) return;

		dynamicBlocks.push_back({ position, dynamicBlockType, dynamicBlockDir });
		Object* newBlockObj(nullptr);
		switch (dynamicBlockType)
		{
		case LeverSwitch:
			newBlockObj = Lever::Create(ObjectMgr, ObjectType::DynamicBlock, dynamicBlockType, dynamicBlockDir);
			break;
		case BasicChest:
			newBlockObj = Chest::Create(ObjectMgr, ObjectType::DynamicBlock, dynamicBlockType, dynamicBlockDir);
			break;
		}
		newBlockObj->GetComponent<TransformComponent>()->SetPosition(position);
		ObjectMgr->AddObject(ObjectType::DynamicBlock, newBlockObj);
	}
}

void EditScene::SaveStage(const char* saveStage)
{
	HANDLE hFile(nullptr);
	string path = "../../Reference/MapData/"; path += saveStage; path += ".dat";

	int len(MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, NULL, 0));
	wstring wpath(len, 0);
	MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, &wpath[0], len);

	hFile = CreateFile(wpath.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(EngineCore::GetInstance()->GetWindowHandle(), L"Save Fail", _T("Fail"), MB_OK);
		return;
	}

	DWORD dwByte(0);
	DWORD SBSize(staticBlocks.size());
	DWORD DBSize(dynamicBlocks.size());
	WriteFile(hFile, &SBSize, sizeof(DWORD), &dwByte, nullptr);
	WriteFile(hFile, &DBSize, sizeof(DWORD), &dwByte, nullptr);
	for (auto& block : staticBlocks) WriteFile(hFile, &block, sizeof(SB), &dwByte, nullptr);
	for (auto& block : dynamicBlocks) WriteFile(hFile, &block, sizeof(DB), &dwByte, nullptr);

	CloseHandle(hFile);
	MessageBox(EngineCore::GetInstance()->GetWindowHandle(), L"Save Success", _T("Success"), MB_OK);
}

void EditScene::LoadStage(const char* loadStage)
{
	HANDLE hFile(nullptr);
	string path = "../../Reference/MapData/"; path += loadStage; path += ".dat";

	int len(MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, NULL, 0));
	wstring wpath(len, 0);
	MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, &wpath[0], len);

	hFile = CreateFile(wpath.c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(EngineCore::GetInstance()->GetWindowHandle(), L"Load Fail", _T("Fail"), MB_OK);
		return;
	}
	
	ObjectMgr->ClearList(ObjectType::StaticBlock); staticBlocks.clear(); staticBlocks.shrink_to_fit();
	ObjectMgr->ClearList(ObjectType::DynamicBlock); dynamicBlocks.clear(); dynamicBlocks.shrink_to_fit();

	DWORD dwByte(0), dwSBTot(0), dwDBTot(0);
	DWORD SBSize(0), DBSize(0);
	SB newSBlock; DB newDBlock;

	if (!ReadFile(hFile, &SBSize, sizeof(DWORD), &dwByte, nullptr)) return;
	if (!ReadFile(hFile, &DBSize, sizeof(DWORD), &dwByte, nullptr)) return;

	for (DWORD i = 0; i < SBSize; ++i)
	{
		if (!ReadFile(hFile, &newSBlock, sizeof(SB), &dwByte, nullptr)) return;

		auto sBlock = StaticBlock::Create(ObjectMgr, ObjectType::StaticBlock, newSBlock.Type, newSBlock.Dir);
		sBlock->GetComponent<TransformComponent>()->SetPosition(newSBlock.Pos);

		ObjectMgr->AddObject(ObjectType::StaticBlock, sBlock);
		staticBlocks.push_back(newSBlock);
	}

	for (DWORD i = 0; i < DBSize; ++i)
	{
		if (!ReadFile(hFile, &newDBlock, sizeof(DB), &dwByte, nullptr)) return;

		auto dBlock = Lever::Create(ObjectMgr, ObjectType::DynamicBlock, newDBlock.Type, newDBlock.Dir);
		dBlock->GetComponent<TransformComponent>()->SetPosition(newDBlock.Pos);

		ObjectMgr->AddObject(ObjectType::DynamicBlock, dBlock);
		dynamicBlocks.push_back(newDBlock);
	}

	CloseHandle(hFile);
	MessageBox(EngineCore::GetInstance()->GetWindowHandle(), L"Load Success", _T("Success"), MB_OK);
}

void EditScene::Free()
{
	Safe_Release(ObjectMgr);
	Safe_Release(CameraMgr);
	Safe_Release(CollisionSys);
	staticBlocks.clear();
	staticBlocks.shrink_to_fit();
	Scene::Free();
}