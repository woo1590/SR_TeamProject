#include "pch.h"
#include "Slot.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

Slot* Slot::Create(ObjectManager* owner)
{
	auto* instance = new Slot(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT Slot::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetPosition(WINCX * 0.5f, WINCY * 0.9f);
	
	renderer->SetScale(0.3f, 0.3f);
	renderer->SetTexture(L"slot");

	return S_OK;
}