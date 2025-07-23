#pragma once

#include "Object.h"

class Stage2Icon : public Object
{
private:
	Stage2Icon(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static Stage2Icon* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

