#include "pch.h"
#include "InventoryPlayer.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "RenderSystem.h"
#include "GraphicDevice.h"
#include "ObjectManager.h"
#include "BaseCharacter.h"
#include "CameraComponent.h"

InventoryPlayer* InventoryPlayer::Create(ObjectManager* owner)
{
	auto instance = new InventoryPlayer(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT InventoryPlayer::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	renderer->SetRenderType(UIRenderType::Inventory);
	transform->SetPosition(300.f,350.f);

	rtv = new RenderTargetView();

	if (FAILED(rtv->CreateTexture(GraphicDevice::GetInstance()->GetDevice(),
		512, 512, D3DFMT_A8R8G8B8))) return E_FAIL;

	rtv->clearColor = D3DCOLOR_ARGB(0, 0, 0, 0);

	renderer->SetTexture(rtv->texture);

	Object* cam = owner->GetObjectList(ObjectType::UICamera).front();
	if (!cam) return E_FAIL;

	rtv->camera = cam->GetComponent<CameraComponent>();

	EngineCore::GetInstance()->GetRenderSystem()->RegisterRTV(rtv);

	return S_OK;
}

void InventoryPlayer::UpdateRendererList()
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

void InventoryPlayer::Update(float dt)
{
	Object::Update(dt);
}

void InventoryPlayer::Late_Update(float dt)
{
	Object::Late_Update(dt);
	UpdateRendererList();
}

void InventoryPlayer::Free()
{
	Object::Free();
	EngineCore::GetInstance()->GetRenderSystem()->UnRegisterRTV(rtv);
}