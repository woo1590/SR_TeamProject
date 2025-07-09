#pragma once

#include "Object.h"

BEGIN(Engine)
class UIRenderer;
class FontComponent;
END

class WorldTooltip : public Object
{
private:
	WorldTooltip(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static WorldTooltip* Create(ObjectManager* owner);
	HRESULT Ready_Object();

private:
	FontComponent* font = nullptr;
	UIRenderer* renderer = nullptr;
};

