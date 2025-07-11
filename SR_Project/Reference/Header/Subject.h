#pragma once

#include "IObserver.h"

BEGIN(Engine)

template<typename EventT>
class Subject
{
public:
	void Attach(IObserver<EventT>* obs)
	{
		if (find(observers.begin(), observers.end(), obs) == observers.end())
			observers.push_back(obs);
	}

	void Detach(IObserver<EventT>* obs)
	{
		observers.erase(remove(observers.begin(), observers.end(), obs), observers.end());
	}

protected:
	void Notify(const EventT& event)
	{
		for (auto* obs : observers)
			obs->OnNotify(event);
	}

private:
	vector<IObserver<EventT>*> observers;
};

END