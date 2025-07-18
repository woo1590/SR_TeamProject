#pragma once

#include "Object.h"

class Arrows : public Object
{
private:
	Arrows(ObjectManager* owner): Object(owner, ObjectType::UI) {}

public:
	static Arrows* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;

private:
	int count = 100;
};

