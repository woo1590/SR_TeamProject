#pragma once

#include "Object.h"

class BossIcon : public Object
{
private:
	BossIcon(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static BossIcon* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

