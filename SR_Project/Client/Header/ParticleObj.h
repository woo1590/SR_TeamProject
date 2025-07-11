#pragma once

#include "Object.h"

class ParticleObj :public Object
{
private:
	ParticleObj(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static ParticleObj* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;
};

