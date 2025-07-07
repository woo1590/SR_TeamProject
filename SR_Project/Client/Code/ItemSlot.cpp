#include "pch.h"
#include "ItemSlot.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "SlotComponent.h"
#include "HoverComponent.h"

ItemSlot* ItemSlot::Create(ObjectManager* owner)
{
	auto* instance = new ItemSlot(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ItemSlot::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.2f, 0.2f);

	auto base = AddComponent<UIRenderer>();
	auto highlight = AddComponent<UIRenderer>();

	base->SetRenderType(UIRenderType::Inventory);
	highlight->SetRenderType(UIRenderType::Inventory);

	auto hover = AddComponent<HoverComponent>();
	auto slot = AddComponent<SlotComponent>();

	base->SetTexture(L"itemslot");
	highlight->SetTexture(L"gearslot_hover");
	highlight->SetScale(0.22f, 0.22f);

	highlight->SetVisible(false);

	slot->BindRenderers(base, highlight);
	slot->SetSlotType(SlotType::Item);

	return S_OK;
}