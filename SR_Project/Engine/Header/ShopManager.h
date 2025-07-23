#pragma once

BEGIN(Engine)

class UIManager;
class InventoryManager;
class Object;
class SlotComponent;

class ENGINE_DLL ShopManager :public Base
{
public:
	using CreateItemCallBack = function<Object* (ItemType)>;

public:
	explicit ShopManager(UIManager* owner) :ui(owner) {}

public:
	void BindCreateCallBack(CreateItemCallBack&& cb) { onCreateItem = move(cb); }
	SlotComponent* GetSelectedSlot() const { return selected; }
	void RegisterShopSlot(Object* slotObj);
	void StockItem(ItemType type, int slotIdx);

	bool BuyItem(int shopSlotIdx);
	bool SellItem(int invSlotIdx);

	void SelectSlot(SlotComponent* newSlot);
	void DeselectAll();

	void Update(float dt);
	void Free() override {}

private:
	UIManager* ui = nullptr;
	array<optional<ItemType>, 10> shopItems;

	vector<SlotComponent*> shopSlots;
	CreateItemCallBack onCreateItem;

	SlotComponent* selected = nullptr;
};

END