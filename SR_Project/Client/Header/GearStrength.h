#pragma once

#include "Object.h"

class GearStrength : public Object
{
private:
	GearStrength(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static GearStrength* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

