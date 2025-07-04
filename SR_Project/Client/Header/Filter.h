#pragma once

#include "Object.h"

class Filter : public Object
{
private:
	Filter(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static Filter* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

