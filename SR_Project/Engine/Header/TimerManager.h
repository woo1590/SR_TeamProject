#pragma once

BEGIN(Engine)

class Timer;
class ENGINE_DLL TimerManager : public Base
{
private:
    TimerManager();
    virtual ~TimerManager();

public:
    HRESULT AddTimer(const std::wstring timerTag);
    void Set_DeltaTime(const std::wstring timerTag);
    _float Get_DeltaTime(const std::wstring timerTag);

    static TimerManager* Create();
private:
    std::unordered_map<std::wstring, Timer*> TimerMap;

    Timer* FindTimer(const std::wstring timerTag);
    void Free()override;
};

END