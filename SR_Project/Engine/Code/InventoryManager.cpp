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
#include "UIManager.h"
#include "InventoryComponent.h"

void InventoryManager::RegisterSlot(Object* slotObj, SlotItemType acceptType, Object* plusObj)
{
	slotObjs.push_back(slotObj);

	auto slotComp = slotObj->GetComponent<SlotComponent>();
	assert(slotComp && "Slot missing SlotComponent");

	slotComp->SetAllowedType(acceptType);

	if (plusObj)
		slotToPlusMap[slotComp] = plusObj;

	if (acceptType == SlotItemType::Potion)
	{
		if (quickSlotCount < 3)
			quickSlots[quickSlotCount++] = slotComp;
	}

	if (auto hover = slotObj->GetComponent<HoverComponent>())
		hover->SetRightClickCallBack([this]() { this->RightClick();});
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
	if (!selected || !selected->HasItem()) return;

	auto itemObj = selected->GetItem();
	auto itemComp = itemObj->GetComponent<ItemComponent>();
	if (!itemComp) return;

	const SlotItemType sourceSlotType = selected->GetAllowedType();

	if (sourceSlotType == SlotItemType::Any)
		HandleEquipAction(itemComp);
	else
		HandleUnEquipAction(itemComp);
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

void InventoryManager::ClearFromQuickSlot(Object* item)
{
	for (auto* quickSlot : quickSlots)
	{
		if (quickSlot && quickSlot->GetItem() == item)
		{
			quickSlot->ClearItem();
			break;
		}
	}
}

void InventoryManager::ApplyFilter(optional<SlotItemType> type)
{
	curFilter = type;

	// 1) 모든 가방 슬롯 수집
	vector<SlotComponent*> bagSlots;
	for (auto* obj : slotObjs)
	{
		auto* slot = obj->GetComponent<SlotComponent>();
		if (slot && slot->GetAllowedType() == SlotItemType::Any)
			bagSlots.push_back(slot);
	}

	// 2) 슬롯에 있던 아이템과 이전 unslottedItems 합치기
	vector<Object*> allItems = move(unslottedItems);
	unslottedItems.clear();
	for (auto* slot : bagSlots)
	{
		if (auto* itm = slot->GetItem())
		{
			allItems.push_back(itm);
			slot->ClearItem();
		}
	}

	// 3) 카테고리별 분류
	vector<Object*> matched, others;
	for (auto* obj : allItems)
	{
		auto* ic = obj->GetComponent<ItemComponent>();
		SlotItemType cat = GetSlotCategory(ic->GetItemType());
		if (!type.has_value() || cat == *type)
			matched.push_back(obj);
		else
			others.push_back(obj);
	}

	// 4) 매치된 아이템부터 슬롯에 재배치
	size_t i = 0;
	for (auto* obj : matched)
	{
		if (i >= bagSlots.size())
		{
			unslottedItems.push_back(obj);
			continue;
		}
		auto* slot = bagSlots[i++];
		slot->SetItem(obj);
		obj->GetComponent<UIRenderer>()->SetVisible(true);
		auto pos = slot->GetOwner()->GetComponent<TransformComponent>()->GetPosition();
		obj->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);
	}

	// 5) 나머지 숨기기
	for (auto* obj : others)
	{
		unslottedItems.push_back(obj);
		if (auto* r = obj->GetComponent<UIRenderer>())
			r->SetVisible(false);
	}
}

bool InventoryManager::InsertItem(Object* item)
{
	auto* emptySlot = FindFirstEmptySlot();
	if (!emptySlot) return false;

	emptySlot->SetItem(item);

	auto pos = emptySlot->GetOwner()->GetComponent<TransformComponent>()->GetPosition();
	item->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);
	return true;
}

void InventoryManager::BindInventory(ItemActionCallBack&& equipCallBack, ItemActionCallBack&& unequipCallBack, CreateItemCallBack&& createCallBack)
{
	OnEquip = move(equipCallBack);
	OnUnEquip = move(unequipCallBack);
	OnCreateItem = move(createCallBack);
	
	player->GetComponent<InventoryComponent>()->SetOnItemAdded([this](ItemType type) {ItemAdded(type);});
}

SlotComponent* InventoryManager::FindSlotByType(SlotItemType typeToFind)
{
	for (auto slotObj : slotObjs)
	{
		if (!slotObj) continue;

		auto slotComp = slotObj->GetComponent<SlotComponent>();
		if (!slotComp) continue;

		if (slotComp->GetAllowedType() == typeToFind)
			return slotComp;
	}
	return nullptr;
}

bool InventoryManager::RemoveItemFromSelectedSlot(ItemType& out)
{
	if (!selected || !selected->HasItem()) return false;

	Object* itemObj = selected->GetItem();
	ItemComponent* itemComp = itemObj->GetComponent<ItemComponent>();
	if (!itemComp)
		return false;

	out = itemComp->GetItemType();

	selected->ClearItem();

	DeselectAll();
	return true;
}

void InventoryManager::ItemAdded(ItemType type)
{
	Object* itemObj = OnCreateItem(type);
	if (!itemObj) return;
	
	auto itemComp = itemObj->GetComponent<ItemComponent>();

	if (OnEquip)
		itemComp->SetEquipCallBack([this, type](Object*) {OnEquip(type); });
	if (OnUnEquip)
		itemComp->SetUnEquipCallBack([this, type](Object*) {OnUnEquip(type); });
	
	InsertItem(itemObj);
}

void InventoryManager::HandleEquipAction(ItemComponent* itemComp)
{
	bool isSwap = false;
	SlotComponent* targetSlot = FindTargetEquipSlot(itemComp->GetItemType(),isSwap);

	if (!targetSlot) return;

	if (isSwap)
	{
		auto oldItemComp = targetSlot->GetItem()->GetComponent<ItemComponent>();
		SwapItems(selected, targetSlot);
		oldItemComp->UnEquip(player);
		itemComp->Equip(player);
	}
	else
	{
		MoveItem(selected, targetSlot);
		itemComp->Equip(player);
	}
	DeselectAll();
}

void InventoryManager::HandleUnEquipAction(ItemComponent* itemComp)
{
	SlotComponent* targetSlot = FindFirstEmptySlot();
	if (targetSlot)
	{
		MoveItem(selected, targetSlot);
		itemComp->UnEquip(player);
		DeselectAll();
	}
}

void InventoryManager::MoveItem(SlotComponent* from, SlotComponent* to)
{
	Object* item = from->GetItem();
	from->ClearItem();
	to->SetItem(item);
}

void InventoryManager::SwapItems(SlotComponent* from, SlotComponent* to)
{
	Object* itemFrom = from->GetItem();
	Object* itemTo = to->GetItem();
	from->SetItem(itemTo);
	to->SetItem(itemFrom);
}

SlotComponent* InventoryManager::FindTargetEquipSlot(ItemType type, bool& isSwap)
{
	const SlotItemType requiredEquipType = GetSlotCategory(type);
	SlotComponent* emptyTarget = nullptr;
	SlotComponent* fullTarget = nullptr;
	isSwap = false;

	if (requiredEquipType == SlotItemType::Potion)
	{
		for (auto quickSlot : quickSlots)
		{
			if (quickSlot && !quickSlot->HasItem())
			{
				emptyTarget = quickSlot;
				break;
			}
		}
		if (!emptyTarget && quickSlots.back() != nullptr)
			fullTarget = quickSlots.back();
	}
	else
	{
		for (auto& obj : slotObjs)
		{
			auto slot = obj->GetComponent<SlotComponent>();
			if (slot && slot->GetAllowedType() == requiredEquipType)
			{
				if (!slot->HasItem())
				{
					emptyTarget = slot;
					break;
				}
				fullTarget = slot;
			}
		}
	}
	if (emptyTarget) return emptyTarget;

	isSwap = true;
	return fullTarget;
}


void InventoryManager::Update(float dt)
{
	const auto& input = EngineCore::GetInstance()->GetInputSystem();

	UIRenderType curRenderType = UIRenderer::GetCurRenderType();

	for (int i = 0; i < quickSlots.size(); ++i)
	{
		auto* quickSlot = quickSlots[i];
		if (!quickSlot) continue;

		Object* item = quickSlot->GetItem();
		if (!item) continue;

		auto* renderer = item->GetComponent<UIRenderer>();
		auto* transform = item->GetComponent<TransformComponent>();
		auto* itemComp = item->GetComponent<ItemComponent>();

		if (!renderer || !transform) continue;
		
		const auto& baseScale = itemComp->GetOriginalScale();

		if (curRenderType == UIRenderType::MainGame)
		{
			renderer->SetRenderType(UIRenderType::MainGame);
			const _vec2& pos = quickSlotPos[i];
			transform->SetPosition(pos.x, pos.y);
			transform->SetScale(baseScale.x * 0.6f,baseScale.y * 0.6f);
		}
		else
		{
			renderer->SetRenderType(UIRenderType::Inventory);
			const _vec2& pos = quickSlotPosInv[i];
			transform->SetPosition(pos.x, pos.y);
			transform->SetScale(baseScale.x,baseScale.y);
		}
	}

	for (auto& [slot, plusObj] : slotToPlusMap)
	{
		if (!plusObj) continue;

		auto* plusRenderer = plusObj->GetComponent<UIRenderer>();
		if (!plusRenderer) continue;

		plusRenderer->SetVisible(!slot->HasItem());
	}
}