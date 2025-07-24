#pragma once

#include "Object.h"

class Atri : public Object
{
private:
	Atri(ObjectManager* owner) :Object(owner, ObjectType::UI) {}

public:
	static Atri* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void SetEmotion(Emotion emotion);
	void Update(float dt) override;

private:
	bool isAnimating = false;
	float animTimer = 0.f;
	float animDur = 0.15f;

	_vec2 startScale{1.8f, 1.8f};
	_vec2 targetScale{2.f, 2.f};
};