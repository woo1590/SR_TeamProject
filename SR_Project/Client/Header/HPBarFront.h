#pragma once

#include "Object.h"

class HPBarFront : public Object
{
private:
	HPBarFront(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static HPBarFront* Create(ObjectManager* owner);
	HRESULT Ready_Object();
	void Update(float dt) override;

	void OnHPChanged(int newHp);

private:
	int prevHp = -1;
	float dmgCooldown = 0.f;
	static constexpr float dmgInterval = 0.1f;
};