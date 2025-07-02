#pragma once

BEGIN(Engine)

template<typename EventT>
class IObserver
{
public:
	virtual void OnNotify(const EventT& event) = 0;
};

END