#pragma once

#include "Object.h"

class PigItem : public Object
{
private:
	PigItem(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static PigItem* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

