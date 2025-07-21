#include "pch.h"
#include "InventoryCam.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "RenderSystem.h"
#include "EngineCore.h"
#include "Player.h"

InventoryCam* InventoryCam::Create(ObjectManager* owner)
{
	auto* instance = new InventoryCam(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT InventoryCam::Ready_Object()
{
	if (FAILED(CameraActor::Ready_Object()))
		return E_FAIL;

	auto camera = GetComponent<CameraComponent>();

	camera->SetProjectionType(CameraComponent::ProjectionType::Orthographic);
	camera->SetOrthoSize(15.f, 15.f);

	return S_OK;
}

void InventoryCam::Late_Update(float dt)
{
	auto camTf = GetComponent<TransformComponent>();

	constexpr float distance = 3.f;
	constexpr float heightOffset = 1.f;

	camTf->SetPosition({0.f, heightOffset, +distance});
	camTf->LookAt({0.f, heightOffset, 0.f});
	Object::Update(dt);
}