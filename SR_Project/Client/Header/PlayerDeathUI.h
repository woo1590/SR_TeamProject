#pragma once

#include "Object.h"

class PlayerDeathUI : public Object
{
private:
	PlayerDeathUI(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static PlayerDeathUI* Create(ObjectManager* owner);
	HRESULT Ready_Object();

};

