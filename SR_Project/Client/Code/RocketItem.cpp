#include "pch.h"
#include "RocketItem.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"

RocketItem* RocketItem::Create(ObjectManager* owner)
{
	auto* instance = new RocketItem(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT RocketItem::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.3f, 0.3f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetTexture(L"inventory_rocket");
	renderer->SetRenderType(UIRenderType::Inventory);

	auto info = AddComponent<InfoComponent<ItemInfo>>();
	auto item = AddComponent<ItemComponent>();
	item->SetItemType(ItemType::RocketItem);
	item->SetOriginalScale({0.3f, 0.3f});

	return S_OK;
}