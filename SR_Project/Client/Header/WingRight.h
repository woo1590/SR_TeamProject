#pragma once

#include "Object.h"

class WingRight : public Object
{
private:
	WingRight(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static WingRight* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

