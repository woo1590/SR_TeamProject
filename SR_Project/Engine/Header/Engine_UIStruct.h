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
	struct ItemInfo 
	{
		bool visible = true;
		float size = 1.f;
		_vec3 scale = { 1.f, 1.f, 1.f };
		_vec3 position = { 0.f, 0.f, 0.f };
		bool pivotEnable = true;
		_vec3 pivot = { 0.f, 0.f, 0.f };
		_vec3 rotation = { 0.f, 0.f, 0.f };
		std::wstring material;
		Engine::RENDER_ID renderId = Engine::RENDER_ID::Render_NonAlpha;
	};
	struct EnemyInfo
	{
		int level;
		int curHp;
		int maxHp;
		int power = 10;
		float attackDelay;
		float speed = 5.f;
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
		Inventory, Equipment, Gear, Quick, Storage, Count
	};
}