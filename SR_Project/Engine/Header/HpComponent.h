#pragma once

#include "ObjectComponent.h"
#include "Object.h"
#include "IObserver.h"

BEGIN(Engine)

class PlayerInfoComponent;
class UIRenderer;

class ENGINE_DLL HpComponent : public ObjectComponent, public IObserver
{
private:
	explicit HpComponent(Object* owner) : ObjectComponent(owner) {}

public:
	static HpComponent* Create(Object* owner);
	
	HRESULT Ready_Component(Object* owner);
	void AttachPlayerInfo(PlayerInfoComponent* info);

	void OnNotify(const NotifyEvent& event) override;
	void Update(float dt) override;

private:
	void ApplyRatio(UIRenderer* renderer, float ratio);

private:
	int maxHp = 1, curHp = 1;
	float targetRatio = 1.f;
	float curRatio = 1.f;
	const float speed = 6.f;

	UIRenderer* front = nullptr;
};

END