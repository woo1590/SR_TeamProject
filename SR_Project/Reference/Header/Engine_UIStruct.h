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
		float speed = 15.f;
	};

	enum class UIRenderType { None, MainGame, Inventory, WorldMap, Loading, DeathUI, QuestUI, Always };
	enum class ItemType{ Sword, Spear, Armor, Bow, CrossBow, Potion, Enchant, Costume, Projectile, None,};
	enum class Rarity { Default, Rare};

	enum class SlotItemType {Any, MeleeWeapon, Armor, RangeWeapon, Potion};

	struct ItemStat
	{
		std::wstring description; // "마비 지속시간"
		std::wstring unit;  // ex) "초", "%"
		std::wstring iconKey;    // 스탯 옆에 표시될 아이콘의 텍스처 키
	};

	struct TooltipLine
	{
		std::wstring text;
		FontType fontType;
		Color color;
		std::wstring iconkey;
	};
	
	struct ItemInfo 
	{
		std::wstring name;
		std::wstring renderKey;
		ItemType type;
		Rarity rarity;
		float value;

		std::wstring flavorText;   // 주 효과 설명
		std::vector<ItemStat> stats;
		std::wstring bgText;    // 배경이야기 
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

	struct SkillInfo
	{
		int level = 1;
		int MaxTarget = 1;
		float DamagePercent = 1.f;
		float SkillRange = 10.f;
	};
}