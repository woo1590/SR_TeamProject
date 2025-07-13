#pragma once

#include "Object.h"

class DeathFrame : public Object
{
private:
	DeathFrame(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static DeathFrame* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;

	void Reset();

private:
	int deathCount = 3;

	float fadeElapsed = 0.f;
	float fadeDuration = 0.5f;
	float countElapsed = 0.f;
	float appearDelay = 0.5f;
	bool appeared = false;

	bool destroyAfterCount = false;


	UIRenderType prevRenderType = UIRenderType::None;
};

