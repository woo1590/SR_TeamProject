#pragma once

#include "Object.h"
#include "TransformComponent.h"
#include "FontComponent.h"
#include "UIRenderer.h"

class LevelUpEffect : public Object
{
private:
	LevelUpEffect(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static LevelUpEffect* Create(ObjectManager* owner);
	HRESULT Ready_Object();
	void Update(float dt) override;

private:
	float elapsedTime = 0.f;
	float duration = 2.f;
	float fadeInEndTime = 0.3f;
	float fadeOutStartTime = 1.7f;

	TransformComponent* transform = nullptr;
	UIRenderer* renderer = nullptr;
	FontComponent* font = nullptr;
};

