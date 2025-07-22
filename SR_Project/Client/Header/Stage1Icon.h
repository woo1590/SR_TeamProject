#pragma once

#include "Object.h"

class Stage1Icon : public Object
{
private:
	Stage1Icon(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static Stage1Icon* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

