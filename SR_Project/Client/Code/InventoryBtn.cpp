#include "pch.h"
#include "InventoryBtn.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ButtonComponent.h"
#include "HoverComponent.h"
#include "EngineCore.h"
#include "RenderSystem.h"
#include "InputSystem.h"

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

	button->SetOnClick([]() {
		EngineCore::GetInstance()->GetRenderSystem()->SetUIRenderState(UIRenderType::Inventory);
		});

	hover->SetUpdateCallBack([this](float dt) {
		if (EngineCore::GetInstance()->GetInputSystem()->IsKeyPressed(KEY::I))
			EngineCore::GetInstance()->GetRenderSystem()->SetUIRenderState(UIRenderType::Inventory);
		});

	return S_OK;
}

