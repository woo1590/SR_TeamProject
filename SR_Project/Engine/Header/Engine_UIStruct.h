#pragma once

namespace Engine
{
	struct PlayerInfo
	{
		int level = 1;
		float curHp = 400.f;
		float maxHp = 400.f;
		int curExp = 0;
		int maxExp = 10;
		float power = 10.f;
		float speed = 15.f;
		int gold = 0;
	};

	enum class UIRenderType { None, MainGame, Inventory, WorldMap, Loading, DeathUI, QuestUI,Shop, Always };
	enum class ItemType{ Sword, Spear, Armor, Bow, CrossBow, Potion, FishingItem, BoostItem, PigItem, GhostCloak, RocketItem, Enchant, Costume, Projectile, HpPotion, None};
	enum class Rarity { Default, Rare};

	enum class SlotItemType {Any, MeleeWeapon, Armor, RangeWeapon, Potion, Enchant, Costume, Projectile, None};
	struct ItemInfo 
	{
		std::wstring name;
		std::wstring renderKey;
		ItemType type;
		SlotItemType category;
		Rarity rarity;
		int value;
		std::wstring description;
	};

	static const std::unordered_map<ItemType, ItemInfo> itemTable =
	{
		{ItemType::Sword, {L"검", L"inventory_sword", ItemType::Sword, SlotItemType::MeleeWeapon, Rarity::Default, 10,
		L"튼튼하고 믿고 쓸 수 있는 검입니다"}},
		{ItemType::Bow, {L"활", L"inventory_bow", ItemType::Bow, SlotItemType::RangeWeapon, Rarity::Default, 10,
		L"단순하지만 균형이 잡혀 있는 무기입니다. 호박 목장의 사냥꾼들은 활은 다른 잡동사니들과 다르게 실망을 시키지 않는다고 말합니다."}},
		{ItemType::Armor, {L"늑대 방어구", L"inventory_wolfarmor",ItemType::Armor, SlotItemType::Armor, Rarity::Default, 1, L"많은 전사들은 전투할 때 늑대의 머리를 뒤집어 쓰고 적들에게 공포감을 줬습니다."}},
		{ItemType::RocketItem, {L"폭죽 화살", L"inventory_rocket",ItemType::RocketItem, SlotItemType::Potion, Rarity::Default, 30, L"TNT의 폭팔하는 가루가 화살의 고속 이동과 결합 되었습니다. 잘못될 일이 뭐가 있겠어요?"}},
		{ItemType::BoostItem, {L"신속의 부츠", L"inventory_boost",ItemType::BoostItem, SlotItemType::Potion, Rarity::Default, 5, L"효과부여를 통해 신속한 움직임을 가능하게 하는 부츠입니다. 지금처럼 모든 게 불확실한 때에 유용합니다."}},
		{ItemType::FishingItem, {L"낚싯대", L"inventory_fishing",ItemType::FishingItem, SlotItemType::Potion, Rarity::Default, 2, L"진정한 모험자가 신뢰할 수 있는 동료인 낚싯대는 낚시에만 유용한 것은 아닙니다."}},
		{ItemType::PigItem, {L"아기 돼지", L"pig", ItemType::PigItem, SlotItemType::Costume, Rarity::Default, 0, L"지금 이 즐거운 꿀꿀 소리는 여러분이 내는 건가요, 아님 이 사랑스러운 아기 돼지가 내는 건가요? 새로운 반려동물이 얼마나 귀여운지 정말 깨물어 주고 싶네요! 하지만 그러면 안되겠죠."}},
		{ItemType::Spear, {L"창", L"inventory_spear",ItemType::Spear, SlotItemType::MeleeWeapon, Rarity::Default, 10, L"이 창은 먼 거리에도 닿아 넓은 범위에 쓸 수 있는 유일한 무기입니다."}},
		{ItemType::CrossBow, {L"석궁", L"inventory_crossbow",ItemType::CrossBow, SlotItemType::RangeWeapon, Rarity::Default, 10, L"석궁은 우민들이 즐겨 쓰는 원거리 무기입니다. 주로 약탈자들이 들고 다닙니다."}},
		{ItemType::GhostCloak, {L"유령 망토",L"inventory_ghostcloak",ItemType::GhostCloak, SlotItemType::Potion, Rarity::Default, 5, L"잠시 유령 형태가 되어 몹을 통과하고 일부 피해를 흡수합니다."}},
		{ItemType::HpPotion, {L"Hp 포션",L"hp_potion",ItemType::HpPotion,SlotItemType::None,Rarity::Default,100,L"HP 10 회복"}},
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
		GOLD_Changed,
	};

	template<typename T>
	struct UIEvent
	{
		UIEventType type;
		T info;
	};

	enum class SlotType
	{
		Inventory, Equip, Gear , Item , Quick, Storage, ShopSlot,Count
	};

	struct SkillInfo
	{
		int level = 1;
		int MaxTarget = 1;
		float DamagePercent = 1.f;
		float SkillRange = 10.f;
	};

	enum class Emotion {p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23};

	struct DialogLine
	{
		std::wstring text;
		Emotion emotion = Emotion::p6;
	};
}