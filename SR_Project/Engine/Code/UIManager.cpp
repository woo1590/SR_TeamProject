#include "EnginePCH.h"
#include "UIManager.h"
#include "InventoryManager.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "QuestSystem.h"

UIManager* UIManager::Create(Scene* owner)
{
	auto* instance = new UIManager(owner);
	return (FAILED(instance->Ready_UIManager())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT UIManager::Ready_UIManager()
{
	inventory = new InventoryManager(this);
	quest = new QuestSystem(this);

	quest->InitQuests();
	return  S_OK;
}

void UIManager::Update(float dt)
{
	if (inventory)
		inventory->Update(dt);
	if (quest)
		quest->Update(dt);
}

void UIManager::Free()
{
	Safe_Release(inventory);
	Safe_Release(quest);
}