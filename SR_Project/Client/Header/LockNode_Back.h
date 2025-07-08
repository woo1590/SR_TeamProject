#pragma once

#include "Object.h"

class LockNode_Back : public Object
{
private:
	LockNode_Back(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static LockNode_Back* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

