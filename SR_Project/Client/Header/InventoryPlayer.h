#pragma once

#include "Object.h"

class InventoryPlayer : public Object
{
private:
	InventoryPlayer(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static InventoryPlayer* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};
