#pragma once

#include "ObjectComponent.h"
#include "Subject.h"

BEGIN(Engine)

class HealthComponent : public ObjectComponent, public Subject
{
public:
	void Damage(float amount)
	{
		cur = (cur > amount) ? cur - amount : 0.f;
		HPInfo info{cur, maxVal};
		Notify(&info);
	}

	void Free() override
	{
		NotifyDestory();
		ObjectComponent::Free();
	}

private:
	struct HPInfo { float cur, max; };
	float cur = 100.f, maxVal = 100.f;
};

END