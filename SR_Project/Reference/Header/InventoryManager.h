#pragma once

BEGIN(Engine)
class SlotComponent;
class UIManager;
class Object;

class ENGINE_DLL InventoryManager: public Base
{
public:
	explicit InventoryManager(UIManager* owner) :uiMgr(owner) {}

public:
	void Update(float dt);

	void RegisterSlot(Object* slotObj, SlotItemType acceptType, Object* plusObj = nullptr);
	void SelectSlot(SlotComponent* newSlot);
	void DeselectAll();
	void RightClick();

	SlotComponent* GetSelectedSlot() const { return selected; }
	SlotComponent* FindFirstEmptySlot();
	void ClearFromQuickSlot(Object* item);
	
	void ApplyFilter(optional<ItemType> type);
	bool InsertItem(Object* item);
	void Free() override {}

	inline SlotItemType GetSlotCategory(ItemType type)
	{
		static const unordered_map<ItemType, SlotItemType> map =
		{
			{ItemType::Sword,   SlotItemType::MeleeWeapon},
			{ItemType::Spear,   SlotItemType::MeleeWeapon},
			{ItemType::Armor,   SlotItemType::Armor},
			{ItemType::Bow,     SlotItemType::RangeWeapon},
			{ItemType::CrossBow,SlotItemType::RangeWeapon},
			{ItemType::Potion,  SlotItemType::Potion},
		};
		auto it = map.find(type);
		if (it != map.end()) return it->second;
		return SlotItemType::Any;
	}

private:
	UIManager* uiMgr = nullptr;
	vector<Object*> slotObjs;
	SlotComponent* selected = nullptr;
	optional<ItemType> curFilter;
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
};

END