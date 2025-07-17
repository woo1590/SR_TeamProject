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
	// 1. 기본 정보 확인
	if (!selected || !selected->HasItem()) return;

	Object* itemObj = selected->GetItem();
	auto itemComp = itemObj->GetComponent<ItemComponent>();
	if (!itemObj) return;

	auto MoveItem = [&](SlotComponent* from, SlotComponent* to)
		{
			from->ClearItem();
			to->SetItem(itemObj);
			auto pos = to->GetOwner()->GetComponent<TransformComponent>()->GetPosition();
			itemObj->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);
			DeselectAll();
		};

	// 두 슬롯의 아이템을 교체
	auto SwapItems = [&](SlotComponent* slotA, SlotComponent* slotB)
		{
			Object* itemA = slotA->GetItem();
			Object* itemB = slotB->GetItem();

			slotA->SetItem(itemB);
			slotB->SetItem(itemA);

			if (itemB)
			{
				auto posA = slotA->GetOwner()->GetComponent<TransformComponent>()->GetPosition();
				itemB->GetComponent<TransformComponent>()->SetPosition(posA.x, posA.y);
			}
			if (itemA)
			{
				auto posB = slotB->GetOwner()->GetComponent<TransformComponent>()->GetPosition();
				itemA->GetComponent<TransformComponent>()->SetPosition(posB.x, posB.y);
			}
		};

	// 2. 소스 슬롯과 타겟 슬롯의 타입 결정
	const SlotItemType sourceSlotType = selected->GetAllowedType();
	const SlotItemType requiredEquipType = GetSlotCategory(itemComp->GetItemType());
	auto player = uiMgr->GetScene()->GetObjectManager()->GetFrontObject(ObjectType::Player);

	// 3. 장착 (인벤토리 -> 장비 슬롯)
	if (sourceSlotType == SlotItemType::Any)
	{
		// 아이템에 맞는 빈 장비 슬롯을 찾음
		SlotComponent* emptyTargetSlot = nullptr;
		SlotComponent* fullTargetSlot = nullptr;
		
		if (requiredEquipType == SlotItemType::Potion)
		{
			// 1순위: 빈 퀵슬롯 찾기
			for (auto quickSlot : quickSlots)
			{
				if (quickSlot && !quickSlot->HasItem())
				{
					emptyTargetSlot = quickSlot;
					break;
				}
			}
			// 2순위: 빈 슬롯이 없다면, 마지막 퀵슬롯을 교체 대상으로 지정
			if (!emptyTargetSlot && quickSlots.back() != nullptr)
				fullTargetSlot = quickSlots.back();
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
						emptyTargetSlot = slot;
						break;
					}
					fullTargetSlot = slot;
				}
			}
		}
		if (emptyTargetSlot)
		{
			MoveItem(selected, emptyTargetSlot);
			itemComp->Equip(player);
		}
		else if (fullTargetSlot)
		{
			Object* oldItemObj = fullTargetSlot->GetItem();
			auto oldItemComp = oldItemObj->GetComponent<ItemComponent>();

			SwapItems(selected, fullTargetSlot);

			oldItemComp->UnEquip(player);
			itemComp->Equip(player);
			DeselectAll();
		}
	}
	// 4. 장착 해제 (장비 슬롯 -> 인벤토리)
	else if (sourceSlotType == requiredEquipType)
	{
		SlotComponent* targetSlot = FindFirstEmptySlot();
		if (targetSlot)
		{
			if (sourceSlotType == SlotItemType::Potion)
				ClearFromQuickSlot(itemObj);
			MoveItem(selected, targetSlot);
			itemComp->UnEquip(player);
		}
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

void InventoryManager::ApplyFilter(optional<ItemType> type)
{
	curFilter = type;

	// 1. 모든 가방 슬롯을 가져온다
	vector<SlotComponent*> bagSlots;
	for (auto& obj : slotObjs)
	{
		auto slot = obj->GetComponent<SlotComponent>();
		if (slot && slot->GetAllowedType() == SlotItemType::Any)
			bagSlots.push_back(slot);
	}

	// 2. 슬롯에 있던 아이템과, 이전에 필터링으로 제외됏던 아이템을 모두 수집한다.
	vector<Object*> allItemsToFilter = unslottedItems;
	unslottedItems.clear();

	for (auto& slot : bagSlots)
	{
		if (Object* itemObj = slot->GetItem())
		{
			allItemsToFilter.push_back(itemObj);
			slot->ClearItem();
		}
	}
	
	// 3. 필터 조건에 따라 아이템들을 "matched" 와 "others"로 분류
	vector<Object*> matchedItems;
	vector<Object*> otherItems;

	if (!type.has_value())
		matchedItems = allItemsToFilter;
	else
	{
		for (auto& item : allItemsToFilter)
		{
			auto itemComp = item->GetComponent<ItemComponent>();
			if (itemComp && itemComp->GetItemType() == *type)
				matchedItems.push_back(item);
			else
				otherItems.push_back(item);
		}
	}

	// 4. "matched" 아이템들을 슬롯에 순서대로 다시 배치
	size_t idx = 0;
	for (auto& item : matchedItems)
	{
		if (idx >= bagSlots.size())
		{
			unslottedItems.push_back(item);
			continue;
		}
		auto slot = bagSlots[idx++];
		slot->SetItem(item);
		item->GetComponent<UIRenderer>()->SetVisible(true);

		auto pos = slot->GetOwner()->GetComponent<TransformComponent>()->GetPosition();
		item->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);
	}

	// 5. 필터에 맞지 않는 "others" 아이템들은 unslottedItems 목록에 보관. 어느 슬롯도 차지하지 않는다
	if (type.has_value())
	{
		unslottedItems.insert(unslottedItems.end(), otherItems.begin(), otherItems.end());
		for (auto item : unslottedItems)
		{
			if (auto renderer = item->GetComponent<UIRenderer>())
				renderer->SetVisible(false);
		}
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