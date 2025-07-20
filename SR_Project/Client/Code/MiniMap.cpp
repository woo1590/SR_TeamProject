#include "pch.h"
#include "MiniMap.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "RenderSystem.h"
#include "GraphicDevice.h"
#include "ObjectManager.h"
#include "Bone.h"
#include "BaseCharacter.h"
#include "CameraComponent.h"

MiniMap* MiniMap::Create(ObjectManager* owner)
{
	auto instance = new MiniMap(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT MiniMap::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();

	transform->SetPosition(64.f, 64.f);

	rtv = new RenderTargetView();

	if (FAILED(rtv->CreateTexture(GraphicDevice::GetInstance()->GetDevice(),
		128, 128, D3DFMT_A8R8G8B8))) return E_FAIL;

	rtv->clearColor = D3DCOLOR_XRGB(25,25,25);

	renderer->SetTexture(rtv->texture);

	Object* cam = owner->GetFrontObject(ObjectType::UICamera);
	if (!cam) return E_FAIL;

	rtv->camera = cam->GetComponent<CameraComponent>();
	
	EngineCore::GetInstance()->GetRenderSystem()->RegisterRTV(rtv);

	return S_OK;
}

void MiniMap::UpdateRendererList()
{
	rtv->renderers.clear();

	Object* playerObj = owner->GetFrontObject(ObjectType::Player);
	
	BaseCharacter* player = static_cast<BaseCharacter*>(playerObj);

	auto& playerBones = player->GetBones();

	for (const auto& pair : playerBones)
	{
		Object* boneObj = pair.second;
		if (boneObj)
		{
			auto renderer = boneObj->GetComponent<MeshRenderer>();
			if (renderer)
				rtv->renderers.push_back(renderer); 
		}
	}
}

void MiniMap::Update(float dt)
{
	Object::Update(dt);

	UpdateRendererList();
}
