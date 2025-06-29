#pragma once

BEGIN(Engine)

class ENGINE_DLL IObserver
{
public:
	virtual void OnNotify(const NotifyEvent& event) = 0;
	virtual void OnDestory() {}
};

END