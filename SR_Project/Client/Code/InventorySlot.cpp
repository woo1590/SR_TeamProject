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
	auto base = AddComponent<UIRenderer>();
	
	base->SetTexture(L"inventoryslot");
	base->SetRenderType(UIRenderType::Inventory);
	base->SetScale(1.2f,1.2f);
	
	auto highlight = AddComponent<UIRenderer>();
	highlight->SetTexture(L"gearslot_hover");
	highlight->SetVisible(false);
	highlight->SetScale(0.6f,0.6f);
	highlight->SetRenderType(UIRenderType::Inventory);

	auto hover = AddComponent<HoverComponent>();
	auto slot = AddComponent<SlotComponent>();

	slot->SetSlotType(SlotType::Inventory);
	slot->BindRenderers(base, highlight);

	return S_OK;
}