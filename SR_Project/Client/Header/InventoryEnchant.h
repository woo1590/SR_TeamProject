#pragma once

#include "Object.h"

class InventoryEnchant : public Object
{
private:
	InventoryEnchant(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static InventoryEnchant* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

