#pragma once

#include "Object.h"

class GearSlotPlus : public Object
{
private:
	GearSlotPlus(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static GearSlotPlus* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

