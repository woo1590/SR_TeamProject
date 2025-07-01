#pragma once

namespace Engine
{
	struct NotifyEvent
	{
		int type;
		void* data;
	};

	struct PlayerInitStat
	{
		int level = 1;
		int maxHp = 100;
		int maxExp = 10;
		float speed = 6.f;
	};

	struct PlayerInfo
	{
		int level;
		int curHp, maxHp;
		int curExp, maxExp;
		float speed;
	};
}