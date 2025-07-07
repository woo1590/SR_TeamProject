#include "pch.h"
#include "PotionFilter.h"

#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "EngineCore.h"
#include "UIManager.h"
#include "InventoryManager.h"

PotionFilter* PotionFilter::Create(ObjectManager* owner)
{
	auto* instance = new PotionFilter(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT PotionFilter::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	base->SetTexture(L"potionfilter");

	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	transform->SetPosition(840.f, 100.f);

	base->SetRenderType(UIRenderType::Inventory);

	button->SetRenderer(base);
	button->SetTextures(L"potionfilter", L"potionfilter_hover");

	button->SetOnClick([]() {
		auto* inv = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetUIManager()->GetInventory();
		inv->ApplyFilter(ItemType::Potion);
		});

	return S_OK;
}