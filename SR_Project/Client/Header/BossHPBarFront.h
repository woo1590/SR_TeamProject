#pragma once

#include "Object.h"

class BossHPBarFront : public Object
{
private:
	BossHPBarFront(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static BossHPBarFront* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

