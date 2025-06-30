#pragma once

#include "Object.h"

class InventoryPanel : public Object
{
private:
	InventoryPanel(ObjectManager* owner, ObjectType type)
		:Object(owner, type) {
	}

public:
	static InventoryPanel* Create(ObjectManager* owner, ObjectType type);
	HRESULT Ready_Object();
};

