#pragma once

#include "Object.h"

class HPBarFront : public Object
{
private:
	HPBarFront(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static HPBarFront* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};