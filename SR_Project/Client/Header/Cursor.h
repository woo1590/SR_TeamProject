#pragma once

#include "Object.h"

class Cursor : public Object
{
private:
	Cursor(ObjectManager* owner, ObjectType type)
		:Object(owner, type) {
	}

public:
	static Cursor* Create(ObjectManager* owner, ObjectType type);
	HRESULT Ready_Object();

	void Update(float dt) override;
};

