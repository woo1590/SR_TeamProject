#include "pch.h"
#include "MiniMapCam.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "ObjectManager.h"

MiniMapCam* MiniMapCam::Create(ObjectManager* owner)
{
	auto* instance = new MiniMapCam(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT MiniMapCam::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto camera    = AddComponent<CameraComponent>();

	camera->SetProjectionType(CameraComponent::ProjectionType::Orthographic);
	camera->SetOrthoSize(50.f, 50.f); 

	auto player = GetScene()->GetObjectManager()->GetFrontObject(ObjectType::Player);
	target = player->GetComponent<TransformComponent>(); 

	return S_OK;
}

void MiniMapCam::Update(float dt)
{
	Object::Update(dt);

	if (!target) return;

	auto transform = GetComponent<TransformComponent>();
	if (!transform) return;

	_vec3 targetPos = target->GetPosition();

	transform->SetPosition(targetPos + offset);
}
