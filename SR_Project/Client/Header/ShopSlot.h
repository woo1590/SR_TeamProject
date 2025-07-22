#pragma once

#include "Object.h"

class ShopSlot : public Object
{
private:
	ShopSlot(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static ShopSlot* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

