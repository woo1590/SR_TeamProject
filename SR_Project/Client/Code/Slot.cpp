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

	renderer->SetScale(0.25f, 0.25f);
	renderer->SetTexture(L"slot");

	return S_OK;
}