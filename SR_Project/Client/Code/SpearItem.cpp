#include "pch.h"
#include "SpearItem.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"

SpearItem* SpearItem::Create(ObjectManager* owner)
{
	auto* instance = new SpearItem(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT SpearItem::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.4f, 0.4f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetTexture(L"inventory_spear");
	renderer->SetRenderType(UIRenderType::Inventory);

	auto info = AddComponent<InfoComponent<ItemInfo>>();
	auto item = AddComponent<ItemComponent>();
	item->SetItemType(ItemType::Spear);
	item->SetOriginalScale({0.4f, 0.4f});

	return S_OK;
}