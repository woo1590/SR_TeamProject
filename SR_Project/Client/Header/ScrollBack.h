#pragma once

#include "Object.h"

class ScrollBack : public Object
{
private:
	ScrollBack(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static ScrollBack* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

