#pragma once

#include "Object.h"

class LoadingUI : public Object
{
private:
	LoadingUI(ObjectManager* owner, ObjectType type)
		:Object(owner, type) {}

public:
	static LoadingUI* Create(ObjectManager* owner, ObjectType type);
	HRESULT Ready_Object();
};

