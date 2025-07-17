#include "pch.h"
#include "CrossBowItem.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"

CrossBowItem* CrossBowItem::Create(ObjectManager* owner)
{
	auto* instance = new CrossBowItem(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT CrossBowItem::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.4f, 0.4f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetTexture(L"inventory_crossbow");
	renderer->SetRenderType(UIRenderType::Inventory);

	auto info = AddComponent<InfoComponent<ItemInfo>>();
	info->SetInfo({L"석궁", L"inventory_crossbow", ItemType::CrossBow,
		Rarity::Default, 10, L"석궁은 우민들이 즐겨 쓰는 원거리 무기입니다. 주로 약탈자들이 들고 다닙니다."});

	auto item = AddComponent<ItemComponent>();
	item->SetOriginalScale({0.4f, 0.4f});

	return S_OK;
}