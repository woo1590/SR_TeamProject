#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

class ItemComponent;
class UIRenderer;

class ENGINE_DLL CoolDownComponent : public ObjectComponent
{
private:
	explicit CoolDownComponent(Object* owner) : ObjectComponent(owner) {}

public:
	static CoolDownComponent* Create(Object* owner);
	HRESULT Ready_Component();

	void Init(ItemComponent* targetItem, UIRenderer* renderer);
	void Update(float dt) override;

private:
	ItemComponent* itemComp = nullptr;
	UIRenderer* cooldownRenderer = nullptr;
};

END