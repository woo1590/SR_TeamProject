#pragma once

#include "Object.h"

class HPBarWhite : public Object
{
private:
	HPBarWhite(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static HPBarWhite* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

