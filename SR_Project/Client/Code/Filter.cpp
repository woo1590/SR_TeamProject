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
	auto base = AddComponent<UIRenderer>();
	base->SetTexture(L"filter");

	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	transform->SetPosition(600.f, 100.f);

	base->SetRenderType(UIRenderType::Inventory);

	button->SetRenderer(base);
	
	button->SetTextures(L"filter", L"filter_hover");

	return S_OK;
}