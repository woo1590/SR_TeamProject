#pragma once

#include "Object.h"
#include "PlayerInfoComponent.h"

class HPBarFront : public Object
{
private:
	HPBarFront(ObjectManager* owner, ObjectType type)
		:Object(owner, type) {}

public:
	static HPBarFront* Create(ObjectManager* owner, ObjectType type, PlayerInfoComponent* playerInfo);
	HRESULT Ready_Object(PlayerInfoComponent* playerInfo);
};