#pragma once

#include "Object.h"

class QuickSlotPlus : public Object
{
private:
	QuickSlotPlus(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static QuickSlotPlus* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

