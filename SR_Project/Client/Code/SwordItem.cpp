#include "pch.h"
#include "SwordItem.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "UIManager.h"
#include "InventoryManager.h"

SwordItem* SwordItem::Create(ObjectManager* owner)
{
	auto* instance = new SwordItem(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT SwordItem::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.4f, 0.4f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetRenderType(UIRenderType::Inventory);
	renderer->SetTexture(L"inventory_sword");

	auto info = AddComponent<InfoComponent<ItemInfo>>();
	auto item = AddComponent<ItemComponent>();
	item->SetItemType(ItemType::Sword);
	item->SetOriginalScale({0.4f, 0.4f});

	return S_OK;
}
