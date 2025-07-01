#pragma once

#include "Object.h"

class InventoryPanel : public Object
{
private:
	InventoryPanel(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static InventoryPanel* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

