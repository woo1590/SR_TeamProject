#pragma once

#include "Object.h"

class MiniMap : public Object
{
private:
	MiniMap(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static MiniMap* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};
