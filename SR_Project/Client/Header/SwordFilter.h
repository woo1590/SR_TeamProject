#pragma once

#include "Object.h"

class SwordFilter : public Object
{
private:
	SwordFilter(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static SwordFilter* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

