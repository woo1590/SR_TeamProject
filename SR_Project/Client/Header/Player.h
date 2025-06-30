#pragma once

#include "Object.h"

class Player : public Object
{
public:
	Player(ObjectManager* owner, ObjectType objType)
		:Object(owner, objType) {
	}

	static Player* Create(ObjectManager* owner, ObjectType objType);

	HRESULT Ready_Object();

	void Update(float dt) override;
};

