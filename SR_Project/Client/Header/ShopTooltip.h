#pragma once

#include "Object.h"
#include "FontComponent.h"
#include "UIRenderer.h"
#include "ShopManager.h"
#include "SlotComponent.h"

class ShopTooltip : public Object
{
private:
	ShopTooltip(ObjectManager* owner) :Object(owner, ObjectType::UI) {}

public:
	static ShopTooltip* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;

	void SetShopManager(ShopManager* _shopMgr) { shopMgr = _shopMgr; }

private:
	FontComponent* font = nullptr;
	UIRenderer* renderer = nullptr;
	ShopManager* shopMgr = nullptr;
	SlotComponent* prevSlot = nullptr;
};

