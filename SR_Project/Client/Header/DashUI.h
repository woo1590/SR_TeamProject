#pragma once

#include "Object.h"

class DashUI : public Object
{
private:
	DashUI(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static DashUI* Create(ObjectManager* owner);
};

