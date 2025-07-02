#pragma once

#include "Object.h"

class ArrowSlot : public Object
{
private:
	ArrowSlot(ObjectManager* owner)
		:Object(owner, ObjectType::UI){}

public:
	static ArrowSlot* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

