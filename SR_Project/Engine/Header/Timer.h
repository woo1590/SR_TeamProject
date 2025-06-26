#pragma once

BEGIN(Engine)

class ENGINE_DLL Timer : public Base
{
private:
	Timer();
	virtual ~Timer();

public:
	HRESULT Ready_Timer();
	void	Update_Timer();

	_float GetDeltaTime()const { return m_fDeltaTime; }

private:
	LARGE_INTEGER m_FrameTime;
	LARGE_INTEGER m_FixTime;
	LARGE_INTEGER m_LastTime;
	LARGE_INTEGER m_CpuTick;

	_float m_fDeltaTime;
public:
	static Timer* Create();
private:
	virtual void Free();
};

END
