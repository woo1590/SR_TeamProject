#include "pch.h"
#include "ShopSlot.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "SlotComponent.h"
#include "HoverComponent.h"

ShopSlot* ShopSlot::Create(ObjectManager* owner)
{
	auto instance = new ShopSlot(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ShopSlot::Ready_Object()
{
	auto tf = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	auto highlight = AddComponent<UIRenderer>();

	tf->SetScale(1.2f, 1.4f);

	base->SetTexture(L"shopslot");
	base->SetRenderType(UIRenderType::Shop);

	highlight->SetTexture(L"gearslot_hover");
	highlight->SetRenderType(UIRenderType::Shop);
	highlight->SetVisible(false);
	highlight->SetScale(0.35f, 0.4f);

	auto hover = AddComponent<HoverComponent>();
	auto slot = AddComponent<SlotComponent>();

	slot->BindRenderers(base, highlight);
	slot->SetSlotType(SlotType::ShopSlot);

	return S_OK;
}