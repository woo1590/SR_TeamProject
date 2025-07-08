#pragma once

#include "Object.h"

class MapBtn : public Object
{
private:
	MapBtn(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static MapBtn* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

