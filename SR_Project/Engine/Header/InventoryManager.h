#pragma once

BEGIN(Engine)
class SlotComponent;
class UIManager;
class Object;
class InventoryComponent;
class ObjectManager;
class ItemComponent;

class ENGINE_DLL InventoryManager: public Base
{
public:
	explicit InventoryManager(UIManager* owner) :uiMgr(owner) {}
	using ItemActionCallBack = function<void(ItemType)>;
	using CreateItemCallBack = function<Object*(ItemType)>;

public:
	void Update(float dt);

	void RegisterSlot(Object* slotObj, SlotItemType acceptType, Object* plusObj = nullptr);
	void SelectSlot(SlotComponent* newSlot);
	void DeselectAll();
	void RightClick();

	SlotComponent* GetSelectedSlot() const { return selected; }
	SlotComponent* FindFirstEmptySlot();
	void ClearFromQuickSlot(Object* item);
	
	void ApplyFilter(optional<SlotItemType> type);
	void ApplyFilter(ItemType type) { ApplyFilter(optional<SlotItemType>(GetSlotCategory(type))); }
	bool InsertItem(Object* item);
	void Free() override {}

	SlotItemType GetSlotCategory(ItemType type) { return itemTable.at(type).category; }

	void BindInventory(ItemActionCallBack&& equipCallBack,ItemActionCallBack&& unequipCallBack, CreateItemCallBack&& createCallBack);
	void SetPlayer(Object* _player) { player = _player; }
	SlotComponent* FindSlotByType(SlotItemType typeToFind);
	bool RemoveItemFromSelectedSlot(ItemType& out);

public:
	void ItemAdded(ItemType type);
	void HandleEquipAction(ItemComponent* itemComp);
	void HandleUnEquipAction(ItemComponent* itemComp);
	void MoveItem(SlotComponent* from, SlotComponent* to);
	void SwapItems(SlotComponent* from, SlotComponent* to);

	SlotComponent* FindTargetEquipSlot(ItemType type, bool& isSwap);

private:
	UIManager* uiMgr = nullptr;
	vector<Object*> slotObjs;
	SlotComponent* selected = nullptr;
	optional<SlotItemType> curFilter;
	array<SlotComponent*, 3> quickSlots{};
	int quickSlotCount = 0;
	array<_vec2, 3> quickSlotPos =
	{
		_vec2{380.f,650.f},
		_vec2{450.f,650.f},
		_vec2{520.f,650.f},
	};
	array<_vec2, 3> quickSlotPosInv =
	{
		_vec2{150.f, 620.f},
		_vec2{300.f, 620.f},
		_vec2{450.f, 620.f},
	};
	unordered_map<SlotComponent*, Object*> slotToPlusMap;
	vector<Object*> unslottedItems;

	// -------------------------------
	ItemActionCallBack OnEquip;
	ItemActionCallBack OnUnEquip;
	CreateItemCallBack OnCreateItem;
	Object* player = nullptr;
};

END