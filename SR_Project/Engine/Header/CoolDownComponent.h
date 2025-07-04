#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

class ENGINE_DLL CoolDownComponent : public ObjectComponent
{
private:
	explicit CoolDownComponent(Object* owner)
		:ObjectComponent(owner) {}

public:
	static CoolDownComponent* Create(Object* owner);
	HRESULT Ready_Component();

	void Start(float _duration);
	void Update(float dt) override;

	bool IsCooling() const { return isCooling; }

private:
	float duration = 0.f;
	float elapsed = 0.f;
	bool isCooling = false;
};

END