#pragma once

namespace Engine
{
	struct NotifyEvent
	{
		int type;
		void* data;
	};

	struct HPData
	{
		int curHp{};
		int maxHp{};
	};
}