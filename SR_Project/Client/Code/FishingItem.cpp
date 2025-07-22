#include "pch.h"
#include "FishingItem.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"

FishingItem* FishingItem::Create(ObjectManager* owner)
{
	auto* instance = new FishingItem(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT FishingItem::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.3f, 0.3f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetTexture(L"inventory_fishing");
	renderer->SetRenderType(UIRenderType::Inventory);

	auto info = AddComponent<InfoComponent<ItemInfo>>();
	auto item = AddComponent<ItemComponent>();
	item->SetItemType(ItemType::FishingItem);
	item->SetOriginalScale({0.3f, 0.3f});

	return S_OK;
}