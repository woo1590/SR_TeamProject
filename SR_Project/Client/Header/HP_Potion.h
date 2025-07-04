#pragma once

#include "Object.h"
#include "InfoComponent.h"

class Player;

class HP_Potion : public Object
{
private:
	HP_Potion(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static HP_Potion* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

