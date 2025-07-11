#pragma once

namespace Engine
{
	struct PlayerInfo
	{
		int level = 1;
		int curHp = 100;
		int maxHp = 100;
		int curExp = 0;
		int maxExp = 10;
		int power = 100;
		float speed = 25.f;
	};

	enum class ItemType{ Sword, Armor, Arrow, Potion, Enchant, Costume };
	enum class Rarity { Default, Rare};

	enum class SlotItemType {Any, Sword, Armor, Arrow, Potion};

	struct ItemInfo 
	{
		std::wstring name;
		std::wstring renderKey;
		ItemType type;
		Rarity rarity;
		int value; 
		std::wstring description;
	};

	struct QuestInfo
	{
		std::wstring title;
		std::wstring desc;
		bool isCompleted = false;
	};

	struct EnemyInfo
	{
		int level;
		int curHp = 100;
		int maxHp = 100;
		int curExp = 0;
		int maxExp = 0;
		int power = 10;
		float attackDelay;
		float speed = 5.f;
	};

	enum class UIEventType
	{
		HP_Changed,
		EXP_Changed,
	};

	template<typename T>
	struct UIEvent
	{
		UIEventType type;
		T info;
	};

	enum class SlotType
	{
		Inventory, Equip, Gear , Item , Quick, Storage, Count
	};

	
}