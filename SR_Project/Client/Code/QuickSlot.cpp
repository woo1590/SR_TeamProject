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
	transform->SetScale(0.25f, 0.25f);

	auto base = AddComponent<UIRenderer>();
	auto highlight = AddComponent<UIRenderer>();

	base->SetTexture(L"quickslot");
	base->SetRenderType(UIRenderType::MainGame);

	highlight->SetTexture(L"quickslot_hover");
	highlight->SetRenderType(UIRenderType::MainGame);
	highlight->SetScale(2.f, 2.f);
	highlight->SetVisible(false);

	auto hover = AddComponent<HoverComponent>();
	auto slot = AddComponent<SlotComponent>();

	slot->BindRenderers(base, highlight);
	slot->SetSlotType(SlotType::Quick);

	return S_OK;
}