#pragma once

#include "ObjectComponent.h"
#include "QuestSystem.h"

BEGIN(Engine)

struct InventoryItemState
{
	ItemType type;
	bool isEquipped = false;
};

class ENGINE_DLL InventoryComponent : public ObjectComponent
{
public:
	using ItemEvent = function<void(ItemType)>;

	explicit InventoryComponent(Object* owner) : ObjectComponent(owner) {}
	static InventoryComponent* Create(Object* owner) { return new InventoryComponent(owner); }

	bool Add(ItemType type)
	{
		auto it = find_if(items.begin(), items.end(), [type](const InventoryItemState& state)
			{return state.type == type; });

		if (it != items.end()) return false;

		items.push_back({type, false});
		if (OnItemAdded)
			OnItemAdded(type);

		return true;
	}
	void Remove(ItemType type)
	{
		items.erase(remove_if(items.begin(), items.end(), [type](const InventoryItemState& state)
			{return state.type == type; }), items.end());
	}

	void Equip(ItemType type)
	{
		auto it = find_if(items.begin(), items.end(), [type](const InventoryItemState& state) 
			{return state.type == type; });

		if (it != items.end())
			it->isEquipped = true;
	}

	void UnEquip(ItemType type)
	{
		auto it = find_if(items.begin(), items.end(), [type](const InventoryItemState& state)
			{return state.type == type; });

		if (it != items.end())
			it->isEquipped = false;
	}

	void SetOnItemAdded(ItemEvent&& cb) {OnItemAdded = move(cb); }
	void SetOnItemRemoved(ItemEvent && cb) {OnItemRemoved = move(cb); }

	vector<ItemType> GetEquippedItems() const
	{
		vector<ItemType> equipped;
		for (const auto& itemState : items)
		{
			if (itemState.isEquipped)
				equipped.push_back(itemState.type);
		}
		return equipped;
	}

	vector<ItemType> GetUnequippedItems() const
	{
		vector<ItemType> unequipped;
		for (const auto& itemState : items)
		{
			if (!itemState.isEquipped)
				unequipped.push_back(itemState.type);
		}
		return unequipped;
	}

	static const vector<ItemType>& GetSupportedItemTypes()
	{
		static vector<ItemType> keys = []
			{
				vector<ItemType> vecItem;
				vecItem.reserve(itemTable.size());
				for (const auto& pair : itemTable)
					vecItem.push_back(pair.first);
				return vecItem;
			}();
		return keys;
	}

	void SetQuestData(const vector<QuestInfo>& _questData) { questData = _questData; }
	const vector<QuestInfo>& GetQuestData() const { return questData; }
	bool HasQuestData() const { return !questData.empty(); }

private:
	vector<InventoryItemState> items;
	ItemEvent OnItemAdded;
	ItemEvent OnItemRemoved;

	vector<QuestInfo> questData;
};

END