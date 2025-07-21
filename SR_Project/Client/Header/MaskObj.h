#pragma once

#include "Object.h"

class MaskObj : public Object
{
private:
	MaskObj(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static MaskObj* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

