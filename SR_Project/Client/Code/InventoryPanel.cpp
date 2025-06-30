#include "pch.h"
#include "InventoryPanel.h"
#include "UIRenderer.h"
#include "TransformComponent.h"

InventoryPanel* InventoryPanel::Create(ObjectManager* owner, ObjectType type)
{
	auto* instance = new InventoryPanel(owner, type);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT InventoryPanel::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetPosition(WINCX * 0.5f, WINCY * 0.5f);
	renderer->SetTexture(L"InventoryPanel");
	//renderer->SetScale(0.25f, 0.125f);
	
	return S_OK;
}
