#pragma once

#include "Object.h"
#include "FontComponent.h"
#include "UIRenderer.h"
#include "InventoryManager.h"
#include "SlotComponent.h"

class TooltipObj : public Object
{
private:
	TooltipObj(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static TooltipObj* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;

	void SetInventoryManager(InventoryManager* invMgr) { inventory = invMgr; }

private:
	FontComponent* font = nullptr;
	UIRenderer* renderer = nullptr;
	InventoryManager* inventory = nullptr;
	SlotComponent* prevSlot = nullptr;
};

