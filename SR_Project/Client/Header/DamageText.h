#pragma once

#include "Object.h"

class DamageText : public Object
{
private:
	DamageText(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static DamageText* Create(ObjectManager* owner);
	HRESULT Ready_Object();
	void Update(float dt) override;

	_vec3 worldPos{};
	_vec2 screenDir{};

private:
	float elapsed = 0.f;
	float lifeTime = 2.f;
	float offsetX = 0.f;
	float offsetY = 0.f;

	static constexpr float kAscend = 35.f;
	static constexpr float kSideMove = 60.f;
};

