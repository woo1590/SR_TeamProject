#pragma once

#include "IObserver.h"

BEGIN(Engine)

class ENGINE_DLL Subject
{
public:
	void Attach(IObserver* observer)
	{
		if (find(observers.begin(), observers.end(), observer) == observers.end())
			observers.push_back(observer);
	}
	void Detach(IObserver* observer)
	{
		observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
	}

protected:
	void Notify(const NotifyEvent& event)
	{
		assert(!observers.empty() && "Subject::Notify - no observers registered");
		for (auto* observer : observers)
			observer->OnNotify(event);
	}

	void NotifyDestroy()
	{
		for (auto* observer : observers)
			observer->OnDestory();
		observers.clear();
	}

private:
	vector<IObserver*> observers;
};

END