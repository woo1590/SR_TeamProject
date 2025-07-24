#pragma once

#include "Object.h"

class DialogRect : public Object
{
private:
	DialogRect(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static DialogRect* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

