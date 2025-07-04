#include "pch.h"
#include "EnchantFilter.h"

#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"

EnchantFilter* EnchantFilter::Create(ObjectManager* owner)
{
	auto* instance = new EnchantFilter(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT EnchantFilter::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	auto highlight = AddComponent<UIRenderer>();
	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	transform->SetPosition(900.f, 100.f);

	base->SetTexture(L"enchantfilter");
	highlight->SetTexture(L"enchantfilter_hover");

	base->SetRenderType(UIRenderType::Inventory);
	highlight->SetRenderType(UIRenderType::Inventory);

	button->SetBase(base);
	button->SetHighlight(highlight);

	return S_OK;
}