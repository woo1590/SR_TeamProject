#include "pch.h"
#include "ArrowFilter.h"

#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"

ArrowFilter* ArrowFilter::Create(ObjectManager* owner)
{
	auto* instance = new ArrowFilter(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ArrowFilter::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	auto highlight = AddComponent<UIRenderer>();
	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	transform->SetPosition(720.f, 100.f);

	base->SetTexture(L"arrowfilter");
	highlight->SetTexture(L"arrowfilter_hover");

	base->SetRenderType(UIRenderType::Inventory);
	highlight->SetRenderType(UIRenderType::Inventory);

	button->SetBase(base);
	button->SetHighlight(highlight);

	return S_OK;
}