#pragma once

#include "Object.h"

class InventorySlot : public Object
{
private:
	InventorySlot(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static InventorySlot* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

