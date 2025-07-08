#pragma once

#include "Object.h"

class WorldMapPanel : public Object
{
private:
	WorldMapPanel(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static WorldMapPanel* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;

private:
	_vec2 mapOffset{};
	_vec2 dragStartPos{};
	_vec2 dragStartOffset{};
	bool isDragging = false;
	const float scale = 0.25f;
};

