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
	info->SetInfo({L"폭죽 화살", L"inventory_rocket",ItemType::Potion, Rarity::Default, 30,
		L"TNT의 폭팔하는 가루가 화살의 고속 이동과 결합 되었습니다. 잘못될 일이 뭐가 있겠어요?"});

	auto item = AddComponent<ItemComponent>();
	item->SetOriginalScale({0.3f, 0.3f});

	return S_OK;
}