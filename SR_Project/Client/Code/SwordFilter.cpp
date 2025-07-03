#include "pch.h"
#include "SwordFilter.h"

#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"

SwordFilter* SwordFilter::Create(ObjectManager* owner)
{
	auto* instance = new SwordFilter(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT SwordFilter::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	auto highlight = AddComponent<UIRenderer>();
	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	transform->SetPosition(660.f, 100.f);
	
	base->SetTexture(L"swordfilter");
	highlight->SetTexture(L"swordfilter_hover");

	base->SetRenderType(UIRenderType::Inventory);
	highlight->SetRenderType(UIRenderType::Inventory);

	button->SetBase(base);
	button->SetHighlight(highlight);

	return S_OK;
}