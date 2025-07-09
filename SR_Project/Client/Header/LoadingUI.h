#pragma once

#include "Object.h"

class LoadingUI : public Object
{
private:
	LoadingUI(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static LoadingUI* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;
	
private:
	float pulseTime = 0.f;
};

