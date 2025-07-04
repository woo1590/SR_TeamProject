#pragma once

#include "Object.h"

class GearSlot : public Object
{
private:
	GearSlot(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static GearSlot* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

