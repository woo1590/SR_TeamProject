#include "pch.h"
#include "ArmorFilter.h"

#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "EngineCore.h"
#include "UIManager.h"
#include "InventoryManager.h"

ArmorFilter* ArmorFilter::Create(ObjectManager* owner)
{
	auto* instance = new ArmorFilter(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ArmorFilter::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	base->SetTexture(L"armorfilter");

	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	transform->SetPosition(780.f, 100.f);

	base->SetRenderType(UIRenderType::Inventory);

	button->SetRenderer(base);
	button->SetTextures(L"armorfilter", L"armorfilter_hover");

	button->SetOnClick([]() {
		auto* inv = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetUIManager()->GetInventory();
		inv->ApplyFilter(ItemType::Armor);
		});

	return S_OK;
}