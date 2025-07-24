#pragma once

#include "Object.h"

class WingLeft : public Object
{
	enum class AnimState {FadingIn, Holding, FadingOut,Finished};
private:
	WingLeft(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static WingLeft* Create(ObjectManager* owner);
	HRESULT Ready_Object();
	void Update(float dt) override;

private:
	AnimState state = AnimState::FadingIn;

	float animTimer = 0.f;

	const float fadeInDur = 0.6f;
	const float holdDur = 1.5f;
	const float fadeOutDur = 0.6f;
};

