#pragma once

#include "Object.h"

class LevelFront : public Object
{
private:
	LevelFront(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static LevelFront* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

