#include "EnginePCH.h"
#include "PlayerInfoComponent.h"

PlayerInfoComponent* PlayerInfoComponent::Create(Object* owner)
{
	auto* instance = new PlayerInfoComponent(owner);
	
	return (FAILED(instance->Ready_Component(owner))) ? Safe_Release(instance), nullptr : instance;
}

HRESULT PlayerInfoComponent::Ready_Component(Object* owner)
{
	info.level = 1;
	info.maxHp = 100;
	info.curHp = info.maxHp;
	info.speed = 6.f;
	info.maxExp = 10;
	info.curExp = 0;

	return S_OK;
}

void PlayerInfoComponent::SetHp(int hp)
{
	info.curHp = clamp(hp, 0, info.maxHp);

	NotifyEvent event{static_cast<int>(NotifyType::HP_Changed), &info};
	Notify(event);
}