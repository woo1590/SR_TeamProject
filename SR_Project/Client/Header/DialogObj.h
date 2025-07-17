#pragma once

#include "Object.h"

class DialogObj : public Object
{
private:
	DialogObj(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static DialogObj* Create(ObjectManager* owner);
	HRESULT Ready_Object();

};

