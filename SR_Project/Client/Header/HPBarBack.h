#pragma once

#include "Object.h"

class HPBarBack : public Object
{
private:
	HPBarBack(ObjectManager* owner, ObjectType type)
		:Object(owner, type) {
	}
	
public:
	static HPBarBack* Create(ObjectManager* owner, ObjectType type);
	HRESULT Ready_Object();
};

