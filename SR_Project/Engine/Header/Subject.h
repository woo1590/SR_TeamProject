#pragma once

#include "IObserver.h"

BEGIN(Engine)

class ENGINE_DLL Subject
{
public:
	void Attach(IObserver* observer) { observers.push_back(observer); }
	void Detach(IObserver* observer) { observers.remove(observer); }

protected:
	void Notify(const NotifyEvent& event) 
	{
		for (auto it = observers.begin(); it != observers.end();)
		{
			IObserver* observer = *it;
			++it;
			observer->OnNotify(event);
		}
	}

	void NotifyDestory() 
	{
		for (auto& observer : observers)
			observer->OnDestory();

		observers.clear();
	}

private:
	list<IObserver*> observers;
};

END