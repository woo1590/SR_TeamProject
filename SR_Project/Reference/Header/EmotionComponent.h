#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

class ENGINE_DLL EmotionComponent: public ObjectComponent
{
private:
	EmotionComponent(Object* owner) : ObjectComponent(owner) {}

public:
	static EmotionComponent* Create(Object* owner);

private:
	bool isAnimating = false;
	float animTimer = 0.f;
	float animDur = 0.15f;

	_vec2 startScale;
	_vec2 targetScale;
};

END