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
	info->SetInfo({L"신속의 부츠", L"inventory_boost", ItemType::Potion,Rarity::Default, 5,
		L"효과부여를 통해 신속한 움직임을 가능하게 하는 부츠입니다. 지금처럼 모든 게 불확실한 때에 유용합니다."});

	auto item = AddComponent<ItemComponent>();
	item->SetOriginalScale({0.35f, 0.35f});

	return S_OK;
}