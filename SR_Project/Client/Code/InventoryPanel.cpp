#include "pch.h"
#include "InventoryPanel.h"
#include "UIRenderer.h"
#include "TransformComponent.h"

InventoryPanel* InventoryPanel::Create(ObjectManager* owner)
{
	auto* instance = new InventoryPanel(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT InventoryPanel::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();

	transform->SetPosition(WINCX * 0.5f, WINCY * 0.5f);

	renderer->SetTexture(L"InventoryPanel");
	renderer->SetRenderType(UIRenderType::Inventory);

	return S_OK;
}
