#pragma once

#include "Object.h"

class FishingItem : public Object
{
private:
	FishingItem(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static FishingItem* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

