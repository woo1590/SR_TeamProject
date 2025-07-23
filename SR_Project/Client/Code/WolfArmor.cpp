#include "pch.h"
#include "WolfArmor.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"

WolfArmor* WolfArmor::Create(ObjectManager* owner)
{
	auto* instance = new WolfArmor(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT WolfArmor::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.4f, 0.4f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetTexture(L"inventory_wolfarmor");
	renderer->SetRenderType(UIRenderType::Inventory);

	auto info = AddComponent<InfoComponent<ItemInfo>>();
	auto item = AddComponent<ItemComponent>();
	item->SetItemType(ItemType::Armor);
	item->SetOriginalScale({0.4f, 0.4f});

	return S_OK;
}