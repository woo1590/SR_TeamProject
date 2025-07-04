#pragma once

#include "Object.h"

class ArmorFilter : public Object
{
private:
	ArmorFilter(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static ArmorFilter* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

