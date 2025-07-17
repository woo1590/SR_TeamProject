#pragma once

#include "Object.h"

class SpearItem : public Object
{
private:
	SpearItem(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static SpearItem* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

