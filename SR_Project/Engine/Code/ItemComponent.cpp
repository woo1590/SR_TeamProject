#include "EnginePCH.h"
#include "ItemComponent.h"
#include "Object.h"
#include "QuestSystem.h"
#include "Scene.h"
#include "uiManager.h"

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
	auto playerInfo = user->GetComponent<InfoComponent<PlayerInfo>>();
	assert(playerInfo && L"missing Info");

	if (itemType == ItemType::Potion)
		playerInfo->AddHp(itemInfo->GetInfo().value);
}

void ItemComponent::Equip(Object* user)
{
	if (onEquipCallback)
		onEquipCallback(user);

	if (!user)return;

	auto quest = owner->GetScene()->GetUIManager()->GetQuestSystem();
	if (!quest) return;

	quest->ReportQuestProgress(QuestType::EquipItem, 1);
}

void ItemComponent::Drop()
{

}