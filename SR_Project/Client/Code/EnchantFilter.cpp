#include "pch.h"
#include "EnchantFilter.h"

#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "EngineCore.h"
#include "UIManager.h"
#include "InventoryManager.h"

EnchantFilter* EnchantFilter::Create(ObjectManager* owner)
{
	auto* instance = new EnchantFilter(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT EnchantFilter::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	base->SetTexture(L"enchantfilter");

	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	transform->SetPosition(900.f, 100.f);

	base->SetRenderType(UIRenderType::Inventory);

	button->SetRenderer(base);
	button->SetTextures(L"enchantfilter", L"enchantfilter_hover");

	return S_OK;
}