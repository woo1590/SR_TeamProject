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
    auto player = owner->GetFrontObject(ObjectType::Player);
    if (!player) return;

    auto targetTf = player->GetComponent<TransformComponent>();
    if (!targetTf) return;

    _vec3 offset{0.f, 1.5f, -3.f};

    auto camTf = GetComponent<TransformComponent>();
    camTf->SetPosition(targetTf->GetPosition() + offset);
    camTf->LookAt(targetTf->GetPosition());

    CameraActor::Late_Update(dt);

    CameraActor::Late_Update(dt);
}