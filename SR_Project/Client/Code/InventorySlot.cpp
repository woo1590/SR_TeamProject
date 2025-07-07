#include "pch.h"
#include "InventorySlot.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "SlotComponent.h"

InventorySlot* InventorySlot::Create(ObjectManager* owner)
{
	auto* instance = new InventorySlot(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT InventorySlot::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	//transform->SetScale(0.7f, 0.7f);

	auto base = AddComponent<UIRenderer>();
	base->SetScale(1.5f, 1.5f);

	auto highlight = AddComponent<UIRenderer>();

	base->SetTexture(L"inventoryslot");
	base->SetRenderType(UIRenderType::Inventory);

	highlight->SetTexture(L"gearslot_hover");
	highlight->SetRenderType(UIRenderType::Inventory);
	highlight->SetVisible(false);
	highlight->SetScale(0.22f, 0.22f);

	auto hover = AddComponent<HoverComponent>();
	auto slot = AddComponent<SlotComponent>();

	slot->BindRenderers(base, highlight);
	slot->SetSlotType(SlotType::Inventory);

	return S_OK;
}