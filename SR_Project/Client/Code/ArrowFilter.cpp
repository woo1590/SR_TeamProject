#include "pch.h"
#include "ArrowFilter.h"

#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "EngineCore.h"
#include "UIManager.h"
#include "InventoryManager.h"

ArrowFilter* ArrowFilter::Create(ObjectManager* owner)
{
	auto* instance = new ArrowFilter(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ArrowFilter::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	base->SetTexture(L"arrowfilter");

	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	transform->SetPosition(720.f, 100.f);
	base->SetRenderType(UIRenderType::Inventory);

	button->SetRenderer(base);
	button->SetTextures(L"arrowfilter", L"arrowfilter_hover");

	button->SetOnClick([]() {
		auto* inv = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetUIManager()->GetInventory();
		inv->ApplyFilter(ItemType::Bow);
		});

	return S_OK;
}