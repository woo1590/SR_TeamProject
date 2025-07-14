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
	info->SetInfo({L"낚싯대", L"inventory_fishing", ItemType::Potion, Rarity::Default, 2,
		L"진정한 모험자가 신뢰할 수 있는 동료인 낚싯대는 낚시에만 유용한 것은 아닙니다."});

	auto item = AddComponent<ItemComponent>();
	item->SetOriginalScale({0.3f, 0.3f});

	return S_OK;
}