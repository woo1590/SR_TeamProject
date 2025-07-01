#pragma once

#include "Object.h"

class Cursor : public Object
{
private:
	Cursor(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static Cursor* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;
};

