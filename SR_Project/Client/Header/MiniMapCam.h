#pragma once

#include "Object.h"
#include "TransformComponent.h"

class MiniMapCam :public Object
{
private:
	MiniMapCam(ObjectManager* owner) : Object(owner, ObjectType::Camera) {}

public:
	static MiniMapCam* Create(ObjectManager* owner);
	HRESULT Ready_Object();
	void Update(float dt) override;

	void SetTarget(TransformComponent* _target) { target = _target; }

private:
	TransformComponent* target = nullptr;
	_vec3 offset = {0.f, 600.f, 0.f};
};

