#pragma once

#include "Object.h"

class Locked_Node : public Object
{
private:
	Locked_Node(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static Locked_Node* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

