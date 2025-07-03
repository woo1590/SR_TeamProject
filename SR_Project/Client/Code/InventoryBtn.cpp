#include "pch.h"
#include "InventoryBtn.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ButtonComponent.h"
#include "HoverComponent.h"

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

	transform->SetPosition(300.f,655.f);
	
	renderer->SetScale(0.25f, 0.25f);
	renderer->SetTexture(L"inventorybtn");

	return S_OK;
}

void InventoryBtn::SetTargetPanel(PanelComponent* panel)
{
	targetPanel = panel;

	if (auto button = GetComponent<ButtonComponent>())
	{
		button->SetOnClick([this] {
			targetPanel->Toggle();
			});
	}
}