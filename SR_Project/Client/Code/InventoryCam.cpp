#include "pch.h"
#include "InventoryCam.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "ObjectManager.h"

InventoryCam* InventoryCam::Create(ObjectManager* owner)
{
	auto* instance = new InventoryCam(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT InventoryCam::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto camera = AddComponent<CameraComponent>();

	auto player = GetScene()->GetObjectManager()->GetFrontObject(ObjectType::Player);
	target = player->GetComponent<TransformComponent>();

	return S_OK;
}

void InventoryCam::Update(float dt)
{
	Object::Update(dt);

	if (!target) return;

	auto transform = GetComponent<TransformComponent>();

	_vec3 targetPos = target->GetPosition();
	_vec3 targetForward = target->GetFoward();

	float distance = 2.5f;
	float height = 1.f;

	_vec3 camPos = targetPos + (targetForward * distance) + _vec3(0.f, height, 0.f);
	transform->SetPosition(camPos);

	_vec3 lookAtPos = targetPos + _vec3(0.f, height, 0.f);
	_vec3 dir = lookAtPos - camPos;
	D3DXVec3Normalize(&dir, &dir);
	transform->SetForward(dir);
}
