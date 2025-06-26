#include "EnginePCH.h"
#include "TimerManager.h"
#include "Timer.h"

TimerManager::TimerManager()
{
}

TimerManager::~TimerManager()
{

}

HRESULT TimerManager::AddTimer(const std::wstring timerTag)
{
	Timer* timer = FindTimer(timerTag);
	if (timer)
		return E_FAIL;

	timer = Timer::Create();
	if (!timer)
		return E_FAIL;

	TimerMap.insert({ timerTag,timer });

	return S_OK;
}

void TimerManager::Set_DeltaTime(const std::wstring timerTag)
{
	Timer* timer = FindTimer(timerTag);

	if (!timer)
		return;

	timer->Update_Timer();
}

_float TimerManager::Get_DeltaTime(const std::wstring timerTag)
{
	Timer* timer = FindTimer(timerTag);

	if (!timer)
		return 0.f;

	return timer->GetDeltaTime();
}

TimerManager* TimerManager::Create()
{
	TimerManager* Instance = new TimerManager;

	return Instance;
}

Timer* TimerManager::FindTimer(const std::wstring timerTag)
{
	auto iter = TimerMap.find(timerTag);

	if (iter != TimerMap.end())
		return iter->second;
	else
		return nullptr;
}

void TimerManager::Free()
{
	std::for_each(TimerMap.begin(), TimerMap.end(), DeleteMap());
	TimerMap.clear();
}
