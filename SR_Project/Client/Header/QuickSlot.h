#pragma once

#include "Object.h"

class QuickSlot :public Object
{
private:
	QuickSlot(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static QuickSlot* Create(ObjectManager* owner);
	HRESULT Ready_Object();

private:
	bool isHovered = false;
};

