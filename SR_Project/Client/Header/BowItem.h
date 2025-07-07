#pragma once

#include "Object.h"

class BowItem : public Object
{
private:
	BowItem(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static BowItem* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

