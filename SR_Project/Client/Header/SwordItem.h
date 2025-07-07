#pragma once

#include "Object.h"

class SwordItem : public Object
{
private:
	SwordItem(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static SwordItem* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

