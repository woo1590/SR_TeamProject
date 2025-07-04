#include "pch.h"
#include "GearSlotPlus.h"
#include "TransformComponent.h"
#include "UIRenderer.h"


GearSlotPlus* GearSlotPlus::Create(ObjectManager* owner)
{
	auto* instance = new GearSlotPlus(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT GearSlotPlus::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	renderer->SetRenderType(UIRenderType::Inventory);
	renderer->SetTexture(L"gearslot_plus");

	return S_OK;
}