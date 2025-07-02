#pragma once

#include "Object.h"

class ExpBarBack : public Object
{
private:
	ExpBarBack(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static ExpBarBack* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

