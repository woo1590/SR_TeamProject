#include "pch.h"
#include "InventoryEnchant.h"
#include "UIRenderer.h"
#include "TransformComponent.h"

InventoryEnchant* InventoryEnchant::Create(ObjectManager* owner)
{
	auto* instance = new InventoryEnchant(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT InventoryEnchant::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	transform->SetScale(0.6f, 0.6f);
	transform->SetPosition(150.f, 50.f);
	renderer->SetTexture(L"icon_enchant");
	renderer->SetRenderType(UIRenderType::Inventory);

	return S_OK;
}
