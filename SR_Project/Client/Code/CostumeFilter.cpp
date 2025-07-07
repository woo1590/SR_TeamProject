#include "pch.h"
#include "CostumeFilter.h"

#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "EngineCore.h"
#include "UIManager.h"
#include "InventoryManager.h"

CostumeFilter* CostumeFilter::Create(ObjectManager* owner)
{
	auto* instance = new CostumeFilter(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT CostumeFilter::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	base->SetTexture(L"costumefilter");

	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	transform->SetPosition(960.f, 100.f);

	base->SetRenderType(UIRenderType::Inventory);

	button->SetRenderer(base);
	button->SetTextures(L"costumefilter", L"costumefilter_hover");

	return S_OK;
}