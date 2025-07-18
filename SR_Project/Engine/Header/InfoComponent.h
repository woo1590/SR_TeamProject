#pragma once

#include "ObjectComponent.h"
#include "Subject.h"

BEGIN(Engine)

template<typename T>
class InfoComponent : public ObjectComponent, public Subject<UIEvent<T>>
{
public:
	explicit InfoComponent(Object* owner) : ObjectComponent(owner) {}

	static InfoComponent* Create(Object* owner) { return new InfoComponent<T>(owner); }

	void SetHp(int hp);
	void AddHp(int amount);
	void AddExp(int amount);
	void SetSpeed(int amount);

	void SetOnZeroHp(function<void()> cb) { onZeroHpCallback = move(cb); }

	const T& GetInfo() const { return info; }

	void Update(float dt) override;

public:
	void SetInfo(const T& _info) { info = _info; }

private:
	T info{};
	function<void()> onZeroHpCallback;
	bool wasDead = false;
};

END

template<typename T>
inline void InfoComponent<T>::SetHp(int hp)
{
	info.curHp = clamp(hp, 0, info.maxHp);
	this->Notify({UIEventType::HP_Changed, info});
}

template<typename T>
inline void InfoComponent<T>::SetSpeed(int amount)
{
	info.speed = amount;
	this->Notify({UIEventType::Speed_Changed, info});
}

template<typename T>
inline void InfoComponent<T>::Update(float dt)
{
	if constexpr (requires(T x) { x.curHp; })
	{
		if (!wasDead && info.curHp <= 0)
		{
			if (onZeroHpCallback)
				onZeroHpCallback();

			wasDead = true;
		}
		else if (info.curHp > 0)
			wasDead = false;
	}
}

template<typename T>
inline void InfoComponent<T>::AddHp(int amount)
{
	SetHp(info.curHp + amount);
}

template<typename T>
inline void InfoComponent<T>::AddExp(int amount)
{
	info.curExp += amount;

	while (info.curExp >= info.maxExp)
	{
		info.curExp -= info.maxExp;
		info.level++;
		info.maxExp += 5;
	}
	
	this->Notify({UIEventType::EXP_Changed, info});
}
