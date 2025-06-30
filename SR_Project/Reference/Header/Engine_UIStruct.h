#pragma once

namespace Engine
{
	struct NotifyEvent
	{
		int type;
		void* data;
	};

	struct PlayerInfo
	{
		int level;
		int curHp, maxHp;
		int curExp, maxExp;
		float speed = 6.f;
	};
}