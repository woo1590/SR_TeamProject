#pragma once

#include "Object.h"

class WolfArmor : public Object
{
private:
	WolfArmor(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static WolfArmor* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

