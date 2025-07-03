#include "pch.h"
#include "QuickSlot.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "SlotComponent.h"
#include "HoverComponent.h"

// System
#include "InputSystem.h"
#include "EngineCore.h"

QuickSlot* QuickSlot::Create(ObjectManager* owner)
{
	auto* instance = new QuickSlot(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT QuickSlot::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	
	base->SetTexture(L"quickslot");
	base->SetScale(0.25f, 0.25f);

	auto highlight = AddComponent<UIRenderer>();
	highlight->SetTexture(L"quickslot_highlight");
	highlight->SetScale(2.f, 2.f);
	highlight->SetVisible(false);

	auto hover = AddComponent<HoverComponent>();
	auto slot = AddComponent<SlotComponent>();
	
	slot->SetSlotType(SlotType::Quick);
	slot->BindRenderers(base, highlight);
	
	return S_OK;
}