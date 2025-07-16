#pragma once

#include "Object.h"

class Angry : public Object
{
private:
	Angry(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static Angry* Create(ObjectManager* owner);
	HRESULT Ready_Object();

};

