#pragma once

#include "Object.h"
#include "TransformComponent.h"

class InventoryCam :public Object
{
private:
	InventoryCam(ObjectManager* owner) : Object(owner, ObjectType::Camera) {}

public:
	static InventoryCam* Create(ObjectManager* owner);
	HRESULT Ready_Object();
	void Update(float dt) override;

	void SetTarget(TransformComponent* _target) { target = _target; }

private:
	TransformComponent* target = nullptr;
	_vec3 offset = {50.f, 0.f, 0.f};
};

