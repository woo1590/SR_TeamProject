#pragma once

#include "Object.h"

class GearStrengthBack : public Object
{
private:
	GearStrengthBack(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static GearStrengthBack* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

