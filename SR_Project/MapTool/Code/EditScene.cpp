#include "pch.h"
#include "EditScene.h"
#include "EngineCore.h"

//system
#include "ObjectManager.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "ImGuiManager.h"

//object
#include "Camera.h"
#include "TestBlock.h"

//component
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"

#include "CubeMesh.h"
#include "Material.h"
#include "GraphicDevice.h"

EditScene::EditScene() : blockType(Dirt)
{
}

EditScene::~EditScene()
{
}

EditScene* EditScene::Create()
{
	EditScene* Instance = new EditScene;
	return Instance;
}

void EditScene::Load()
{
	// EngineCore::GetInstance()->GetImGuiManager()->RegisterWindow(L"MapToolTest", [this]() {this->ImGuiTest();});

	auto cube = CubeMesh::Create();
	auto resource = EngineCore::GetInstance()->GetResourceManager();
	
	resource->LoadTexture(L"../Resource/Texture/Block/DirtBlock.dds", L"DirtBlock", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"DirtBlock", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/GrassBlock.dds", L"GrassBlock", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"GrassBlock", cube);
	
	auto dirtBlockMtrl = Material::Create();
	dirtBlockMtrl->SetTexture(L"DirtBlock");
	resource->LoadMaterial(L"DirtBlock_Mtrl", dirtBlockMtrl);

	auto grassBlockMtrl = Material::Create();
	grassBlockMtrl->SetTexture(L"GrassBlock");
	resource->LoadMaterial(L"GrassBlock_Mtrl", grassBlockMtrl);

	BlockData baseBlock{ {0, 0, 0}, BlockType::Dirt };
	Blocks.push_back(baseBlock);

	ObjectMgr = ObjectManager::Create(this);
	ObjectMgr->AddObject(ObjectType::Camera, Camera::Create(ObjectMgr, ObjectType::Camera));
	ObjectMgr->AddObject(ObjectType::Block, TestBlock::Create(ObjectMgr, ObjectType::Block, BlockType::GrassDirt));
}

void EditScene::Update(float dt)
{
	ObjectMgr->Update(dt);

	wchar_t title[128];
	swprintf_s(title, L"Blocks : %d", (int)Blocks.size());
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

	if (Input->IsKeyPressed(NUM1)) blockType = Dirt;
	if (Input->IsKeyPressed(NUM2)) blockType = GrassDirt;
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
	ImGui::Begin("Simple Window");
	ImGui::Text("Hello, ImGui!");
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

	for (const auto& block : Blocks)
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

	if (!found)
		return;

	_vec3 normal = GetHitNormal(closestHitPoint, selectedBlockPos - _vec3(1, 1, 1), selectedBlockPos + _vec3(1, 1, 1));
	_vec3 newBlockPos = selectedBlockPos + normal * 2.0f;
	PlaceBlock(newBlockPos, blockType);
}

void EditScene::OnRightClick(_vec3& rayOrigin, _vec3& rayDir)
{
	bool found(false);
	int targetIndex(-1);
	float closestDist(FLT_MAX);

	for (int i = 0; i < Blocks.size(); ++i)
	{
		const auto& block = Blocks[i];
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

	ObjectMgr->RemoveObject(ObjectType::Block, (Blocks.begin() + targetIndex)->Pos);
	Blocks.erase(Blocks.begin() + targetIndex);
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
	if (fabs(hitPoint.x - boxMin.x) < 0.01f) return _vec3(-1, 0, 0);
	if (fabs(hitPoint.x - boxMax.x) < 0.01f) return _vec3(1, 0, 0);
	if (fabs(hitPoint.y - boxMin.y) < 0.01f) return _vec3(0, -1, 0);
	if (fabs(hitPoint.y - boxMax.y) < 0.01f) return _vec3(0, 1, 0);
	if (fabs(hitPoint.z - boxMin.z) < 0.01f) return _vec3(0, 0, -1);
	if (fabs(hitPoint.z - boxMax.z) < 0.01f) return _vec3(0, 0, 1);
	
	return _vec3(0, 0, 0);
}

void EditScene::PlaceBlock(const _vec3& position, BlockType type)
{
	for (const auto& block : Blocks)
		if (block.Pos == position)
			return;

	BlockData newBlock{ position, type };
	Blocks.push_back(newBlock);

	auto newBlockObj = TestBlock::Create(ObjectMgr, ObjectType::Block, type);
	auto transform = newBlockObj->GetComponent<TransformComponent>();
	transform->SetPosition(position);
	ObjectMgr->AddObject(ObjectType::Block, newBlockObj);
}

void EditScene::SaveBlock()
{
}

void EditScene::LoadBlock()
{
}

void EditScene::Free()
{
	Safe_Release(ObjectMgr);
	Blocks.clear();
	Scene::Free();
}