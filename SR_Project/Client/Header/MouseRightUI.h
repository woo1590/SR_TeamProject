#pragma once

#include "Object.h"

class MouseRightUI : public Object
{
private:
	MouseRightUI(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static MouseRightUI* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

