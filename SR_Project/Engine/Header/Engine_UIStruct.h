#pragma once

namespace Engine
{
	struct PlayerInfo
	{
		int level = 1;
		float curHp = 100.f;
		float maxHp = 100.f;
		int curExp = 0;
		int maxExp = 10;
		float power = 10.f;
		float speed = 15.f;
	};

	enum class UIRenderType { None, MainGame, Inventory, WorldMap, Loading, DeathUI, QuestUI, Always };
	enum class ItemType{ Sword, Spear, Armor, Bow, CrossBow, Potion, Enchant, Costume, Projectile, None,};
	enum class Rarity { Default, Rare};

	enum class SlotItemType {Any, MeleeWeapon, Armor, RangeWeapon, Potion};
	struct ItemInfo 
	{
		std::wstring name;
		std::wstring renderKey;
		ItemType type;
		Rarity rarity;
		int value;
		std::wstring description;
	};

	struct EnemyInfo
	{
		int level;
		float curHp = 100.f;
		float maxHp = 100.f;
		int curExp = 0;
		int maxExp = 0;
		float power = 10.f;
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

	enum class Emotion { None, Angry, Sad, Happy, Emm, CloseEye, Confuse, Brave};

	struct DialogLine
	{
		std::wstring text;
		Emotion emotion = Emotion::None;
	};
}