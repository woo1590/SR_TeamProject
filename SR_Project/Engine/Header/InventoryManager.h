#pragma once

BEGIN(Engine)
class SlotComponent;
class UIManager;
class Object;

class ENGINE_DLL InventoryManager: public Base
{
public:
	explicit InventoryManager(UIManager* owner)
		:ui(owner) {}

public:
	void Update(float dt);

	void RegisterSlot(Object* slotObj, SlotItemType acceptType);
	void SelectSlot(SlotComponent* newSlot);
	void DeselectAll();

	void RightClick();

	UIManager* GetUI() const { return ui; }
	SlotComponent* GetSelectedSlot() const { return selected; }
	SlotComponent* FindFirstEmptySlot();
	
	void ApplyFilter(optional<ItemType> type);

	bool InsertItem(Object* item);

	void Free() override {}

private:
	UIManager* ui = nullptr;
	vector<Object*> slotObjs;
	SlotComponent* selected = nullptr;
	optional<ItemType> curFilter;
};

END