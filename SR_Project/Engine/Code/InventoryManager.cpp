#include "EnginePCH.h"
#include "InventoryManager.h"
#include "SlotComponent.h"
#include "Object.h"
#include "HoverComponent.h"
#include "ItemComponent.h"
#include "EngineCore.h"
#include "SceneManager.h"
#include "Scene.h"
#include "InputSystem.h"
#include "UIRenderer.h"
#include "ObjectManager.h"
#include "TransformComponent.h"

void InventoryManager::RegisterSlot(Object* slotObj, SlotItemType acceptType)
{
	slotObjs.push_back(slotObj);

	auto slotComp = slotObj->GetComponent<SlotComponent>();
	assert(slotComp && "Slot missing SlotComponent");

	slotComp->SetAllowedType(acceptType);

	if (auto hover = slotObj->GetComponent<HoverComponent>())
	{
		hover->SetRightClickCallBack([this]() {
			this->RightClick();
			});
	}
}

void InventoryManager::SelectSlot(SlotComponent* newSlot)
{
	if (selected && selected != newSlot)
		selected->Deselect();

	selected = newSlot;
	selected->Select();
}

void InventoryManager::DeselectAll()
{
	if (selected)
		selected->Deselect();
	selected = nullptr;
}

void InventoryManager::RightClick()
{
	if (!selected) return;

	Object* itemObj = selected->GetItem();
	if (!itemObj) return;

	auto* itemComp = itemObj->GetComponent<ItemComponent>();
	if (!itemComp) return;

	const ItemType itemType = itemComp->GetItemType();

	auto IsSlotAcceptable = [itemType](SlotItemType type)
		{
			return
				(type == SlotItemType::Sword && itemType == ItemType::Sword) ||
				(type == SlotItemType::Armor && itemType == ItemType::Armor) ||
				(type == SlotItemType::Arrow && itemType == ItemType::Arrow) ||
				(type == SlotItemType::Potion && itemType == ItemType::Potion);
		};

	if (IsSlotAcceptable(selected->GetAllowedType()))
	{
		for (auto* obj : slotObjs)
		{
			auto* slot = obj->GetComponent<SlotComponent>();
			if (!slot || slot->HasItem()) continue;
			if (slot->GetAllowedType() != SlotItemType::Any) continue;

			selected->ClearItem();

			slot->SetItem(itemObj);

			auto pos = slot->GetOwner()->GetComponent<TransformComponent>()->GetPosition();
			itemObj->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);

			DeselectAll();
			return;
		}
	}

	for (auto* obj : slotObjs)
	{
		auto* slot = obj->GetComponent<SlotComponent>();
		if (!slot || slot->HasItem())
			continue;
		if (!IsSlotAcceptable(slot->GetAllowedType()))
			continue;

		selected->ClearItem();
		slot->SetItem(itemObj);

		auto pos = slot->GetOwner()->GetComponent<TransformComponent>()->GetPosition();
		itemObj->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);

		DeselectAll();

		auto* player = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetObjectManager()->GetFrontObject(ObjectType::Player);
		itemComp->Use(player);
		
		return; 
	}
}

SlotComponent* InventoryManager::FindFirstEmptySlot()
{
	for (auto* obj : slotObjs)
	{
		auto* slot = obj->GetComponent<SlotComponent>();
		if (!slot || slot->HasItem())
			continue;

		const auto& type = slot->GetAllowedType();
		if (type == SlotItemType::Any)
			return slot;
	}
	return nullptr;
}

void InventoryManager::ApplyFilter(optional<ItemType> type)
{
	curFilter = type;

	vector<Object*> bagSlots;
	for (auto* obj : slotObjs)
	{
		auto* slot = obj->GetComponent<SlotComponent>();
		if (slot && slot->GetAllowedType() == SlotItemType::Any)
			bagSlots.push_back(obj);
	}

	vector<Object*> matched, others;
	for (auto* obj : bagSlots)
	{
		auto* slot = obj->GetComponent<SlotComponent>();
		Object* itemObj = slot->GetItem();
		if (!itemObj) continue;

		auto* itemComp = itemObj->GetComponent<ItemComponent>();
		if (!itemComp) continue;

		slot->ClearItem();

		((type && itemComp->GetItemType() != *type) ? others : matched).push_back(itemObj);
	}

	size_t idx = 0;
	auto assign = [&](vector<Object*>& list, bool visible)
		{
			for (auto* item : list)
			{
				if (idx >= bagSlots.size()) break;
				auto* slot = bagSlots[idx++]->GetComponent<SlotComponent>();
				slot->SetItem(item);

				auto pos = slot->GetOwner()->GetComponent<TransformComponent>()->GetPosition();
				item->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);
				item->GetComponent<UIRenderer>()->SetVisible(visible);
			}
		};
	assign(matched, true);
	assign(others, !type.has_value());
}

bool InventoryManager::InsertItem(Object* item)
{
	auto* emptySlot = FindFirstEmptySlot();
	if (!emptySlot)
		return false;

	emptySlot->SetItem(item);

	auto pos = emptySlot->GetOwner()->GetComponent<TransformComponent>()->GetPosition();
	item->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);
	return true;
}

void InventoryManager::Update(float dt)
{
	const auto& input = EngineCore::GetInstance()->GetInputSystem();
}