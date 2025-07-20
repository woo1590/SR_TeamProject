#pragma once

#include "CameraActor.h"
#include "TransformComponent.h"

class InventoryCam : public CameraActor
{
private:
	InventoryCam(ObjectManager* owner) : CameraActor(owner, ObjectType::UICamera) {}

public:
	static InventoryCam* Create(ObjectManager* owner);
	HRESULT Ready_Object();
	void Late_Update(float dt) override;
};

