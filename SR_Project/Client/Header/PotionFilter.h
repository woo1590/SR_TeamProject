#pragma once

#include "Object.h"

class PotionFilter : public Object
{
private:
	PotionFilter(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static PotionFilter* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

