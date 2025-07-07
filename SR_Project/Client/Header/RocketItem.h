#pragma once

#include "Object.h"

class RocketItem : public Object
{
private:
	RocketItem(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static RocketItem* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

