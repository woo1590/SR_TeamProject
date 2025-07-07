#include "pch.h"
#include "InventoryEmerald.h"
#include "UIRenderer.h"
#include "TransformComponent.h"

InventoryEmerald* InventoryEmerald::Create(ObjectManager* owner)
{
	auto* instance = new InventoryEmerald(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT InventoryEmerald::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	transform->SetScale(0.4f, 0.5f);
	transform->SetPosition(65.f, 50.f);

	renderer->SetTexture(L"inventory_emerald");
	renderer->SetRenderType(UIRenderType::Inventory);

	return S_OK;
}
