#include "EnginePCH.h"
#include "HpComponent.h"

HpComponent* HpComponent::Create(Object* owner)
{
	auto* instance = new HpComponent(owner);

	instance->SetNotifyType(NotifyType::HP_Changed);

	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

void HpComponent::ExtractValues(const void* data, int& cur, int& max)
{
	const auto* info = static_cast<const PlayerInfo*>(data);
	cur = info->curHp;
	max = info->maxHp;
}