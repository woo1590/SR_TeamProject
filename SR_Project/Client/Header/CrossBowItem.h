#pragma once

#include "Object.h"

class CrossBowItem : public Object
{
private:
	CrossBowItem(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static CrossBowItem* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

