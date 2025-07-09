#include "pch.h"
#include "FilterTooltip.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"


FilterTooltip* FilterTooltip::Create(ObjectManager* owner)
{
	auto* instance = new FilterTooltip(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT FilterTooltip::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	renderer->SetRenderType(UIRenderType::Inventory);

	font = AddComponent<FontComponent>();
	font->SetVisible(false);

	return S_OK;
}