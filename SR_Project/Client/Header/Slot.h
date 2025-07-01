#pragma once

#include "Object.h"

class Slot :public Object
{
private:
	Slot(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static Slot* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

