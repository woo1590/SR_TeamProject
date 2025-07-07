#include "EnginePCH.h"
#include "ItemComponent.h"
#include "Object.h"

ItemComponent* ItemComponent::Create(Object* owner)
{
	auto* instance = new ItemComponent(owner);

	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ItemComponent::Ready_Component()
{
	itemInfo = owner->GetComponent<InfoComponent<ItemInfo>>();
	assert(itemInfo && "ItemComponent needs InfoComponent<ItemInfo>");

	itemType = itemInfo->GetInfo().type;

	return S_OK;
}

void ItemComponent::Use(Object* user)
{
	assert(user && "Use: user is null");

	auto playerInfo = user->GetComponent<InfoComponent<PlayerInfo>>();
	assert(playerInfo && "Use: user missing PlayerInfo");

	const auto& info = itemInfo->GetInfo();

	switch (info.type)
	{
	case ItemType::Sword:
	case ItemType::Armor:
	case ItemType::Arrow:
		Equip(user);
		break;

	case ItemType::Potion:
		playerInfo->AddHp(info.value);
		break;

	default:
		break;
	}
}

void ItemComponent::Equip(Object* user)
{

}

void ItemComponent::Drop()
{

}