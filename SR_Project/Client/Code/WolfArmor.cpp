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
	info->SetInfo({L"늑대방어구",L"inventory_wolfarmor", ItemType::Armor, Rarity::Default, 1, L"많은 전사들은 전투할 때 늑대의 머리를 뒤집어 쓰고 적들에게 공포감을 줬습니다."});

	auto item = AddComponent<ItemComponent>();

	return S_OK;
}