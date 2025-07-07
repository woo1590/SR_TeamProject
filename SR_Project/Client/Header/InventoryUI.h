#pragma once

#include "Object.h"

class InventoryUI : public Object
{
private:
	InventoryUI(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static InventoryUI* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;
};

