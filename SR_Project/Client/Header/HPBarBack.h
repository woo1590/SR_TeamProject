#pragma once

#include "Object.h"

class HPBarBack : public Object
{
private:
	HPBarBack(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}
	
public:
	static HPBarBack* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

