#pragma once

#include "Object.h"

class ExpBarFront : public Object
{
private:
	ExpBarFront(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static ExpBarFront* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

