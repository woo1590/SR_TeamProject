#include "pch.h"
#include "InventoryBtn.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ButtonComponent.h"
#include "HoverComponent.h"
#include "Scene.h"
#include "CameraManager.h"
#include "ObjectManager.h"
#include "Player.h"

InventoryBtn* InventoryBtn::Create(ObjectManager* owner)
{
	auto* instance = new InventoryBtn(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT InventoryBtn::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto hover     = AddComponent<HoverComponent>();
	auto button    = AddComponent<ButtonComponent>();

	button->SetRenderer(renderer);
	transform->SetPosition(300.f,655.f);
	transform->SetScale(0.25f, 0.25f);
	renderer->SetTexture(L"inventorybtn");

	button->SetOnClick([this]() {
		auto player = static_cast<Player*>(owner->GetFrontObject(ObjectType::Player));
		player->SetInventoryMode(true);
		});

	return S_OK;
}

