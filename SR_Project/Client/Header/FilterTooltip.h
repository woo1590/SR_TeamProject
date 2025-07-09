#pragma once

#include "Object.h"
#include "FontComponent.h"

class FilterTooltip : public Object
{
private:
	FilterTooltip(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static FilterTooltip* Create(ObjectManager* owner);
	HRESULT Ready_Object();

private:
	FontComponent* font = nullptr;
};

