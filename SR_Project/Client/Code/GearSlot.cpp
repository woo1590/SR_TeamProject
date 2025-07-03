#include "pch.h"
#include "GearSlot.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "SlotComponent.h"
#include "HoverComponent.h"

GearSlot* GearSlot::Create(ObjectManager* owner)
{
	auto* instance = new GearSlot(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT GearSlot::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base      = AddComponent<UIRenderer>();
	auto highlight = AddComponent<UIRenderer>();

	base->SetRenderType(UIRenderType::Inventory);
	highlight->SetRenderType(UIRenderType::Inventory);

	auto hover = AddComponent<HoverComponent>();
	auto slot  = AddComponent<SlotComponent>();
	
	base->SetTexture(L"gearslot");
	highlight->SetTexture(L"gearslot_hover");

	highlight->SetVisible(false);

	slot->SetSlotType(SlotType::Gear);
	slot->BindRenderers(base, highlight);

	return S_OK;
}