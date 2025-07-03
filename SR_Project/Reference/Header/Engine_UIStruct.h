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
		float speed = 6.f;
	};

	struct EnemyInfo
	{
		int level;
		int curHp;
		int maxHp;
		int power;
		float attackDelay;
		float speed;
	};

	enum class UIEventType
	{
		HP_Changed,
		EXP_Changed
	};

	template<typename T>
	struct UIEvent
	{
		UIEventType type;
		T info;
	};

	enum class SlotType
	{
		Inventory, Equipment, Quick, Storage, Count
	};
}