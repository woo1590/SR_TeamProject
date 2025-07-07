#include "pch.h"
#include "BowItem.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"

BowItem* BowItem::Create(ObjectManager* owner)
{
	auto* instance = new BowItem(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT BowItem::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.4f, 0.4f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetTexture(L"inventory_bow");
	renderer->SetRenderType(UIRenderType::Inventory);

	auto info = AddComponent<InfoComponent<ItemInfo>>();
	info->SetInfo({L"활", L"inventory_bow",ItemType::Arrow, Rarity::Default, 10, L"단순하지만 균형이 잡혀 있는 무기입니다. 호박 목장의 사냥꾼들은 활은 다른 잡동사니들과 다르게 실망을 시키지 않는다고 말합니다."});

	auto item = AddComponent<ItemComponent>();

	return S_OK;
}