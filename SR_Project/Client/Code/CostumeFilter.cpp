#include "pch.h"
#include "CostumeFilter.h"

#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"

CostumeFilter* CostumeFilter::Create(ObjectManager* owner)
{
	auto* instance = new CostumeFilter(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT CostumeFilter::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	auto highlight = AddComponent<UIRenderer>();
	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	transform->SetPosition(960.f, 100.f);

	base->SetTexture(L"costumefilter");
	highlight->SetTexture(L"costumefilter_hover");

	base->SetRenderType(UIRenderType::Inventory);
	highlight->SetRenderType(UIRenderType::Inventory);

	button->SetBase(base);
	button->SetHighlight(highlight);

	return S_OK;
}