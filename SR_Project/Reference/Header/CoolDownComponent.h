#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

class UIRenderer;

class ENGINE_DLL CoolDownComponent : public ObjectComponent
{
private:
	explicit CoolDownComponent(Object* owner)
		:ObjectComponent(owner) {}

public:
	static CoolDownComponent* Create(Object* owner);
	HRESULT Ready_Component();

	void Play();
	void Update(float dt) override;

private:
	UIRenderer* whiteOverlay = nullptr;
	UIRenderer* cooldownBar = nullptr;

	float curTime = 0.f;
	float maxTime = 0.5f;
	bool playing = false;
	
};

END