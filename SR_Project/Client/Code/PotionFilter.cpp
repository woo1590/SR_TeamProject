#include "pch.h"
#include "PotionFilter.h"

#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"

PotionFilter* PotionFilter::Create(ObjectManager* owner)
{
	auto* instance = new PotionFilter(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT PotionFilter::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	auto highlight = AddComponent<UIRenderer>();
	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	transform->SetPosition(840.f, 100.f);

	base->SetTexture(L"potionfilter");
	highlight->SetTexture(L"potionfilter_hover");

	base->SetRenderType(UIRenderType::Inventory);
	highlight->SetRenderType(UIRenderType::Inventory);

	button->SetBase(base);
	button->SetHighlight(highlight);

	return S_OK;
}