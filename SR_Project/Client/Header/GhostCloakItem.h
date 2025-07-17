#pragma once

#include "Object.h"

class GhostCloakItem : public Object
{
private:
	GhostCloakItem(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static GhostCloakItem* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};