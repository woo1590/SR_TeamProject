#pragma once

#include "Object.h"
#include "PanelComponent.h"

enum class ExitBtnType {None, Inventory, WorldMap, Shop};

class ExitBtn : public Object
{
private:
	ExitBtn(ObjectManager* owner)
		:Object(owner,ObjectType::UI) {}

public:
	static ExitBtn* Create(ObjectManager* owner, ExitBtnType type);
	HRESULT Ready_Object();

private:
	ExitBtnType btnType = ExitBtnType::None;
};

