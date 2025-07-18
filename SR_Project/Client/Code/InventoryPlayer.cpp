#include "pch.h"
#include "InventoryPlayer.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "RenderSystem.h"

InventoryPlayer* InventoryPlayer::Create(ObjectManager* owner)
{
	auto instance = new InventoryPlayer(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT InventoryPlayer::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	renderer->SetTexture(EngineCore::GetInstance()->GetRenderSystem()->GetInventoryTexture());
	renderer->SetRenderType(UIRenderType::Inventory);

	transform->SetPosition(200.f, 600.f);

	return S_OK;
}