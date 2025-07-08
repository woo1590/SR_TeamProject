#pragma once

#include "Object.h"

class MapNode_Front : public Object
{
private:
	MapNode_Front(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static MapNode_Front* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

