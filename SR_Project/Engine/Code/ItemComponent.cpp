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

void ItemComponent::SetItemType(ItemType _type)
{
	itemType = _type;
	auto infoComp = owner->GetComponent<InfoComponent<ItemInfo>>();
	if (infoComp)
		infoComp->SetInfo(itemTable.at(itemType));
}

void ItemComponent::Use(Object* user)
{
	if (isCoolDownItem && IsCoolDown()) return;

	if (isCoolDownItem)
		coolDownTimer = coolDownDur;

	if (onUseCallBack)
		onUseCallBack(user);
}

void ItemComponent::Equip(Object* user, optional<int> slotIdx)
{
	isEquipped = true;
	equippedSlotIdx = slotIdx;

	if (onEquipCallback)
		onEquipCallback(user);

	if (!user)return;

	auto quest = owner->GetScene()->GetUIManager()->GetQuestSystem();
	if (!quest) return;

	quest->ReportQuestProgress(QuestType::EquipItem, 1);
}

void ItemComponent::UnEquip(Object* user)
{
	isEquipped = false;
	equippedSlotIdx.reset();

	if (unEquipCallback)
		unEquipCallback(user);
}

void ItemComponent::SetCoolDown(bool _isCool, float _dur)
{
	isCoolDownItem = _isCool;
	coolDownDur = _dur;
}

float ItemComponent::GetCoolDownRatio() const
{
	if (coolDownDur <= 0.f) return 0.f;
	return clamp(coolDownTimer / coolDownDur, 0.f, 1.f);
}

void ItemComponent::Update(float dt)
{
	if (isCoolDownItem && coolDownTimer > 0.f)
		coolDownTimer -= dt;
}
