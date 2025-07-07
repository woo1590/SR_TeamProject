#pragma once

#include "Object.h"

class ItemSlot : public Object
{
private:
	ItemSlot(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static ItemSlot* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

