#pragma once

#include "ObjectComponent.h"
#include "Subject.h"

BEGIN(Engine)

class ENGINE_DLL PlayerInfoComponent : public ObjectComponent, public Subject
{
public:
	explicit PlayerInfoComponent(Object* owner) : ObjectComponent(owner) {}

	static PlayerInfoComponent* Create(Object* owner) { return new PlayerInfoComponent(owner); }

	void SetHp(int hp)
	{
		curHp = clamp(hp, 0, maxHp);

		HPData hpData{curHp, maxHp};
		NotifyEvent event{static_cast<int>(NotifyType::HP_Changed), &hpData};
		Notify(event);
	}

	int GetCurHp() const { return curHp; }
	int GetMaxHp() const { return maxHp; }

private:
	int curHp = 100;
	int maxHp = 100;
};

END