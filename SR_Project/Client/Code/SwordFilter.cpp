#include "pch.h"
#include "SwordFilter.h"

#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"

#include "Scene.h"
#include "SceneManager.h"
#include "EngineCore.h"
#include "UIManager.h"
#include "InventoryManager.h"

SwordFilter* SwordFilter::Create(ObjectManager* owner)
{
	auto* instance = new SwordFilter(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT SwordFilter::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	base->SetTexture(L"swordfilter");

	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	transform->SetPosition(660.f, 100.f);

	base->SetRenderType(UIRenderType::Inventory);
	
	button->SetRenderer(base);
	button->SetTextures(L"swordfilter", L"swordfilter_hover");

	button->SetOnClick([]() {
		auto* inv = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetUIManager()->GetInventory();
		inv->ApplyFilter(ItemType::Sword);
		});

	return S_OK;
}