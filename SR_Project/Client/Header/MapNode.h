#pragma once

#include "Object.h"

class MapNode : public Object
{
private:
	MapNode(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static MapNode* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

