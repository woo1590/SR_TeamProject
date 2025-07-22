#pragma once

#include "CameraActor.h"
#include "TransformComponent.h"
#include "LeftBtn.h"
#include "RightBtn.h"

class InventoryCam : public CameraActor
{
private:
	InventoryCam(ObjectManager* owner) : CameraActor(owner, ObjectType::UICamera) {}

public:
	static InventoryCam* Create(ObjectManager* owner);
	HRESULT Ready_Object();
	void Late_Update(float dt) override;

	void SetLeftBtn(LeftBtn* btn) { leftBtn = btn; }
	void SetRightBtn(RightBtn* btn) { rightBtn = btn; }

private:
	float yaw = 0.f;
	float lastX = 0.f;
	bool dragging = false;

	static constexpr float rotSpeed = 2.f;
	static constexpr float distance = 3.f;
	static constexpr float heightOffset = 1.f;

	LeftBtn* leftBtn = nullptr;
	RightBtn* rightBtn = nullptr;
};

