#include "EnginePCH.h"
#include "PlayerInfoComponent.h"

PlayerInfoComponent* PlayerInfoComponent::Create(Object* owner)
{
	auto* instance = new PlayerInfoComponent(owner);
	
	return (FAILED(instance->Ready_Component(owner))) ? Safe_Release(instance), nullptr : instance;
}

HRESULT PlayerInfoComponent::Ready_Component(Object* owner)
{
	PlayerInitStat stat;

	info.level = stat.level;
	info.maxHp = stat.maxHp;
	info.curHp = stat.maxHp;
	info.speed = stat.speed;
	info.maxExp = stat.maxExp;
	info.curExp = 0;

	return S_OK;
}


void PlayerInfoComponent::SetHp(int hp)
{
	info.curHp = clamp(hp, 0, info.maxHp);

	NotifyEvent event{static_cast<int>(NotifyType::HP_Changed), &info};
	Notify(event);
}

void PlayerInfoComponent::AddHp(int amount)
{
	SetHp(info.curHp + amount);
}

void PlayerInfoComponent::AddExp(int amount)
{
	info.curExp += amount;

	while (info.curExp >= info.maxExp)
	{
		info.curExp -= info.maxExp;
		info.level++;
		info.maxExp += 5;
	}

	NotifyEvent event{static_cast<int>(NotifyType::EXP_Changed), &info};
	Notify(event);
}