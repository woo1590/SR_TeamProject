#include "pch.h"
#include "Filter.h"

#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"

Filter* Filter::Create(ObjectManager* owner)
{
	auto* instance = new Filter(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT Filter::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base      = AddComponent<UIRenderer>();
	auto highlight = AddComponent<UIRenderer>();
	auto hover     = AddComponent<HoverComponent>();
	auto button    = AddComponent<ButtonComponent>();

	transform->SetPosition(600.f, 100.f);

	base->SetTexture(L"filter");
	highlight->SetTexture(L"filter_hover");

	base->SetRenderType(UIRenderType::Inventory);
	highlight->SetRenderType(UIRenderType::Inventory);

	button->SetBase(base);
	button->SetHighlight(highlight);

	return S_OK;
}