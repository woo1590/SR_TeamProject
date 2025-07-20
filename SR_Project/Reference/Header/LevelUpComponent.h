#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)
class TransformComponent;
class UIRenderer;
class FontComponent;

class ENGINE_DLL LevelUpComponent : public ObjectComponent
{
private:
	explicit LevelUpComponent(Object* owner) : ObjectComponent(owner) {}

public:
	static LevelUpComponent* Create(Object* owner);

	HRESULT Ready_Component() override;
	void Update(float dt) override;

private:
	float elapsedTime = 0.f;
	float dur = 2.f;
	float fadeInEndTime = 0.3f;
	float fadeOutStartTime = 1.7f;

	TransformComponent* transform = nullptr;
	UIRenderer* renderer = nullptr;
	FontComponent* font = nullptr;
};

END