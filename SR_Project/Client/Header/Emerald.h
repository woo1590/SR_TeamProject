#pragma once

#include "Object.h"

class Emerald : public Object
{
private:
	Emerald(ObjectManager* owner)
		:Object(owner,ObjectType::UI){}

public:
	static Emerald* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	
};

