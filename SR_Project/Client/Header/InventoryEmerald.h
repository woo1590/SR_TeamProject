#pragma once

#include "Object.h"

class InventoryEmerald : public Object
{
private:
	InventoryEmerald(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static InventoryEmerald* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

