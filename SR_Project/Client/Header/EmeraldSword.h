#pragma once

#include "Object.h"

class EmeraldSword : public Object
{
private:
	EmeraldSword(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static EmeraldSword* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

