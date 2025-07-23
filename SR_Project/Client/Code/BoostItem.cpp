#include "pch.h"
#include "BoostItem.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"

BoostItem* BoostItem::Create(ObjectManager* owner)
{
	auto* instance = new BoostItem(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT BoostItem::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.35f, 0.35f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetTexture(L"inventory_boost");
	renderer->SetRenderType(UIRenderType::Inventory);

	auto info = AddComponent<InfoComponent<ItemInfo>>();
	auto item = AddComponent<ItemComponent>();
	item->SetItemType(ItemType::BoostItem);
	item->SetOriginalScale({0.35f, 0.35f});

	return S_OK;
}