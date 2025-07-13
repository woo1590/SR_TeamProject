#pragma once

#include "Object.h"

class BossHPBarFront : public Object
{
private:
	BossHPBarFront(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static BossHPBarFront* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;

private:
	float elapsedTime = 0.f;
	float fadeDuration = 2.f;
	bool fadeCompleted = false;
};

