#pragma once

#include "Object.h"

class LeftBtn : public Object
{
private:
	LeftBtn(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static LeftBtn* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

