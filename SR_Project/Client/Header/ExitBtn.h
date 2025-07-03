#pragma once

#include "Object.h"
#include "PanelComponent.h"

class ExitBtn : public Object
{
private:
	ExitBtn(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static ExitBtn* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

