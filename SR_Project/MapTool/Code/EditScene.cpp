#include "pch.h"
#include "EditScene.h"
#include "EngineCore.h"

//system
#include "ObjectManager.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "InputSystem.h"

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

EditScene::EditScene()
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
	auto cube = CubeMesh::Create();
	auto resource = EngineCore::GetInstance()->GetResourceManager();
	
	resource->LoadMesh(L"DirtBlock", cube);
	resource->LoadTexture(L"../Resource/Texture/Block/DirtBlock.dds", L"DirtBlock", TEXTURE::Tex_Cube);
	
	auto dirtBlockMtrl = Material::Create();
	resource->LoadMaterial(L"DirtBlock_Mtrl", dirtBlockMtrl);
	dirtBlockMtrl->SetTexture(L"DirtBlock");

	BlockData baseBlock{ {0, 0, 0}, BlockType::Dirt };
	Blocks.push_back(baseBlock);

	ObjectMgr = ObjectManager::Create(this);
	ObjectMgr->AddObject(ObjectType::Camera, Camera::Create(ObjectMgr, ObjectType::Camera));
	ObjectMgr->AddObject(ObjectType::Block, TestBlock::Create(ObjectMgr, ObjectType::Block));
}

void EditScene::Update(float dt)
{
	ObjectMgr->Update(dt);

	wchar_t title[128];
	swprintf_s(title, L"Blocks : %d", (int)Blocks.size());
	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), title);

	auto Input = EngineCore::GetInstance()->GetInputSystem();
	if (Input->IsKeyDown(LBUTTON))
	{
		_vec3 rayOrigin, rayDir;
		MakePickingRay(rayOrigin, rayDir);
		OnLeftClick(rayOrigin, rayDir);
	}
}

void EditScene::Late_Update(float dt)
{
	ObjectMgr->Late_Update(dt);
}

void EditScene::Unload()
{
}

void EditScene::MakePickingRay(_vec3& outRayOrigin, _vec3& outRayDir)
{
	LPDIRECT3DDEVICE9 device = GraphicDevice::GetInstance()->GetDevice();

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(EngineCore::GetInstance()->GetWindowHandle(), &pt);
	int mouseX(pt.x), mouseY(pt.y);

	D3DVIEWPORT9 vp;
	device->GetViewport(&vp);

	_matrix projMatrix;
	device->GetTransform(D3DTS_PROJECTION, &projMatrix);
	float px = ((2.0f * mouseX) / vp.Width - 1.0f) / projMatrix._11;
	float py = ((-2.0f * mouseY) / vp.Height + 1.0f) / projMatrix._22;

	_vec3 rayDirView(px, py, 1.f);

	_matrix viewMatrix;
	device->GetTransform(D3DTS_VIEW, &viewMatrix);

	_matrix invView;
	D3DXMatrixInverse(&invView, NULL, &viewMatrix);

	outRayOrigin = ObjectMgr->GetFrontObject(ObjectType::Camera)->GetComponent<TransformComponent>()->GetPosition();

	D3DXVec3TransformNormal(&outRayDir, &rayDirView, &invView);
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
		_vec3 min = block.Pos;
		_vec3 max = block.Pos + _vec3(2, 2, 2);

		rayDir = block.Pos - rayOrigin;
		D3DXVec3Normalize(&rayDir, &rayDir);

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

	_vec3 normal = GetHitNormal(closestHitPoint, selectedBlockPos, selectedBlockPos + _vec3(2, 2, 2));
	_vec3 newBlockPos = selectedBlockPos + normal;
	PlaceBlock(newBlockPos, BlockType::Dirt);
}

bool EditScene::RayIntersectsAABB(const _vec3& rayOrigin, const _vec3& rayDir, const _vec3& boxMin, const _vec3& boxMax, float& outDistance)
{
	float tMin(0.f);
	float tMax(100000.f);

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

			float tNear = min(t1, t2);
			float tFar = max(t1, t2);

			tMin = max(tMin, tNear);
			tMax = min(tMax, tFar);

			if (tMin > tMax)
				return false;
		}
	}

	outDistance = tMin;
	return true;
}

_vec3 EditScene::GetHitNormal(const _vec3& hitPoint, const _vec3& boxMin, const _vec3& boxMax)
{
	const float epsilon(0.1f);

	if (fabs(hitPoint.x - boxMin.x) < epsilon) return _vec3(-2, 0, 0);
	if (fabs(hitPoint.x - boxMax.x) < epsilon) return _vec3(2, 0, 0);
	if (fabs(hitPoint.y - boxMin.y) < epsilon) return _vec3(0, -2, 0);
	if (fabs(hitPoint.y - boxMax.y) < epsilon) return _vec3(0, 2, 0);
	if (fabs(hitPoint.z - boxMin.z) < epsilon) return _vec3(0, 0, -2);
	if (fabs(hitPoint.z - boxMax.z) < epsilon) return _vec3(0, 0, 2);

	return _vec3(0, 0, 0);
}

void EditScene::PlaceBlock(const _vec3& position, BlockType type)
{
	for (const auto& block : Blocks)
		if (block.Pos == position)
			return;

	BlockData newBlock{ position, type };
	Blocks.push_back(newBlock);

	auto newBlockObj = TestBlock::Create(ObjectMgr, ObjectType::Block);
	auto transform = newBlockObj->GetComponent<TransformComponent>();
	transform->SetPosition(position);
	ObjectMgr->AddObject(ObjectType::Block, newBlockObj);
}

void EditScene::Free()
{
	Safe_Release(ObjectMgr);
	Blocks.clear();
	Scene::Free();
}