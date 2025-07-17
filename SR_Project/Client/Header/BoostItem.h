#pragma once

#include "Object.h"

class BoostItem : public Object
{
private:
	BoostItem(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static BoostItem* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};