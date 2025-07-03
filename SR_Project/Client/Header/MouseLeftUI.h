#pragma once

#include "Object.h"

class MouseLeftUI : public Object
{
private:
	MouseLeftUI(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static MouseLeftUI* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

