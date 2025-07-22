#pragma once

#include "Object.h"

class RightBtn : public Object
{
private:
	RightBtn(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static RightBtn* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

