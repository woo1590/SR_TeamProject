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
		int power = 10;
		float speed = 6.f;
	};
	struct ItemInfo 
	{
		float size = 1.f;
		_vec3 scale = { 1.f, 1.f, 1.f };
		_vec3 position = { 0.f, 0.f, 0.f };
		bool pivotEnable = true;
		_vec3 pivot = { 0.f, 0.f, 0.f };
		_vec3 rotation = { 0.f, 0.f, 0.f };
		std::wstring meshType;
		std::wstring material;
		Engine::RENDER_ID renderId = Engine::RENDER_ID::Render_NonAlpha;
		float attackDamage = 10.f;
	};
	struct EnemyInfo
	{
		int level;
		int curHp = 100;
		int maxHp = 100;
		int power = 10;
		float attackDelay;
		float speed = 5.f;
	};
	struct ProjectileInfo {
		ObjectType shooterType;
		float speed = 1.f;
		float size = 1.f;
		_vec3 scale = { 1.f, 1.f, 1.f };
		_vec3 direction = { 0.f, 0.f, 0.f };
		std::wstring meshType;
		std::wstring material;
		Engine::RENDER_ID renderId = Engine::RENDER_ID::Render_NonAlpha;
		float attackDamage = 10.f;
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
		Inventory, Equipment, Gear, Quick, Storage, Count
	};
}