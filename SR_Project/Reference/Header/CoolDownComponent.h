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
	static CoolDownComponent* Create(Object* owner) { return new CoolDownComponent(owner); }

	void Init(ItemComponent* targetItem, UIRenderer* mask);
	void Update(float dt) override;

private:
	ItemComponent* itemComp = nullptr;
	UIRenderer* maskRenderer = nullptr;
};

END