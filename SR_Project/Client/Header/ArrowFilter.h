#pragma once

#include "Object.h"

class ArrowFilter : public Object
{
private:
	ArrowFilter(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static ArrowFilter* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

