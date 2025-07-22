#include "pch.h"
#include "InventoryCam.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "RenderSystem.h"
#include "EngineCore.h"
#include "Player.h"
#include "InputSystem.h"
#include "HoverComponent.h"

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
	camera->SetOrthoSize(distance * 5, distance * 5);

	return S_OK;
}

void InventoryCam::Late_Update(float dt)
{
	const auto& input = EngineCore::GetInstance()->GetInputSystem();
	float direction = 0.f;

	auto playerTf = owner->GetFrontObject(ObjectType::Player)->GetComponent<TransformComponent>();
	auto playerPosY = playerTf->GetPosition().y;

	if (leftBtn)
	{
		auto hover = leftBtn->GetComponent<HoverComponent>();
		if (input->IsKeyDown(KEY::LBUTTON) && hover->IsHovered())
			direction -= 1.f;
	}
	if (rightBtn)
	{
		auto hover = rightBtn->GetComponent<HoverComponent>();
		if (input->IsKeyDown(KEY::LBUTTON) && hover->IsHovered())
			direction += 1.f;
	}
	yaw -= direction * rotSpeed * dt;

	auto tf = GetComponent<TransformComponent>();
	tf->SetPosition({sinf(yaw) * distance, heightOffset + playerPosY, cosf(yaw) * distance});
	tf->LookAt({0.f, heightOffset + playerPosY, 0.f});

	Object::Update(dt);
}