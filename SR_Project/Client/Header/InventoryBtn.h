#pragma once

#include "Object.h"
#include "PanelComponent.h"

class InventoryBtn : public Object
{
private:
	InventoryBtn(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static InventoryBtn* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

