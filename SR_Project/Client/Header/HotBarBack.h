#pragma once

#include "Object.h"

class HotBarBack :public Object
{
private:
	HotBarBack(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static HotBarBack* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt);
};

