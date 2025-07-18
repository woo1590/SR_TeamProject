#pragma once

#include "Object.h"

class CoolDownBar : public Object
{
private:
	CoolDownBar(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static CoolDownBar* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

