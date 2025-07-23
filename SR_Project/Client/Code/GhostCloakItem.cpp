#include "pch.h"
#include "GhostCloakItem.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"

GhostCloakItem* GhostCloakItem::Create(ObjectManager* owner)
{
	auto* instance = new GhostCloakItem(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT GhostCloakItem::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.35f, 0.35f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetTexture(L"inventory_ghostcloak");
	renderer->SetRenderType(UIRenderType::Inventory);

	auto info = AddComponent<InfoComponent<ItemInfo>>();
	auto item = AddComponent<ItemComponent>();
	item->SetItemType(ItemType::GhostCloak);
	item->SetOriginalScale({0.35f, 0.35f});

	return S_OK;
}