#pragma once

#include "Object.h"

class EnchantFilter : public Object
{
private:
	EnchantFilter(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static EnchantFilter* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

