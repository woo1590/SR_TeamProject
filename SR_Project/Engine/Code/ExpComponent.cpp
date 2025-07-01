#include "EnginePCH.h"
#include "ExpComponent.h"

ExpComponent* ExpComponent::Create(Object* owner)
{
	auto* instance = new ExpComponent(owner);

	instance->SetNotifyType(NotifyType::EXP_Changed);
	instance->SetBarDirection(BarDirection::Horizontal);

	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

void ExpComponent::ExtractValues(const void* data, int& cur, int& max)
{
	const auto* info = static_cast<const PlayerInfo*>(data);

	cur = info->curExp;
	max = info->maxExp;
}