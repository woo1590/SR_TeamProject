#include "pch.h"
#include "ArrowSlot.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"

#include "InputSystem.h"

ArrowSlot* ArrowSlot::Create(ObjectManager* owner)
{
	auto* instance = new ArrowSlot(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ArrowSlot::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetScale(0.25f, 0.3f);
	renderer->SetTexture(L"arrow_slot");

	return S_OK;
}