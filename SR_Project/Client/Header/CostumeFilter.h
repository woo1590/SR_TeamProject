#pragma once

#include "Object.h"

class CostumeFilter : public Object
{
private:
	CostumeFilter(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static CostumeFilter* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

