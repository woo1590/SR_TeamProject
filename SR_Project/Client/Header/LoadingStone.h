#pragma once

#include "Object.h"

class LoadingStone : public Object
{
private:
	LoadingStone(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static LoadingStone* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

