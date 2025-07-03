#include "pch.h"
#include "QuickSlotPlus.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

QuickSlotPlus* QuickSlotPlus::Create(ObjectManager* owner)
{
	auto* instance = new QuickSlotPlus(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT QuickSlotPlus::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	renderer->SetScale(0.4f, 0.4f);
	renderer->SetTexture(L"quickslot_plus");

	return S_OK;
}