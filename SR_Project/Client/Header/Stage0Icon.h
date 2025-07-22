#pragma once

#include "Object.h"

class Stage0Icon : public Object
{
private:
	Stage0Icon(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static Stage0Icon* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

