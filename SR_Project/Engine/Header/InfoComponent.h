#pragma once

#include "ObjectComponent.h" 
#include "Subject.h"

BEGIN(Engine)

template<typename T>
class InfoComponent : public ObjectComponent, public Subject<UIEvent<T>>
{
public:
	explicit InfoComponent(Object* owner) : ObjectComponent(owner) {}
	~InfoComponent() override = default;

	static InfoComponent* Create(Object* owner) { return new InfoComponent<T>(owner); }

	template<typename U = T>
	void SetHp(decltype(U::curHp) hp) requires requires(U u) { u.curHp; u.maxHp; };

	template<typename U = T>
	void AddHp(decltype(U::curHp) amount) requires requires(U u) { u.curHp; u.maxHp; };

	template<typename U = T>
	void AddExp(int amount) requires requires(U u) { u.curExp; u.maxExp; u.level; u.maxHp; u.power; };

	template<typename U = T>
	void SetSpeed(decltype(U::speed) amount) requires requires(U u) { u.speed; };

	template<typename U = T>
	void SetGold(decltype(U::gold) amount) requires requires(U u) { u.gold; };

	template<typename U = T>
	void AddGold(decltype(U::gold) amount) requires requires(U u) { u.gold; };

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
template<typename U>
void InfoComponent<T>::SetHp(decltype(U::curHp) hp) requires requires(U u) { u.curHp; u.maxHp; }
{
	info.curHp = std::clamp(hp, static_cast<decltype(U::curHp)>(0), info.maxHp);
	this->Notify({UIEventType::HP_Changed, info});
}

template<typename T>
template<typename U>
void InfoComponent<T>::AddHp(decltype(U::curHp) amount) requires requires(U u) { u.curHp; u.maxHp; }
{
	SetHp(info.curHp + amount);
}

template<typename T>
template<typename U>
void InfoComponent<T>::AddExp(int amount) requires requires(U u) { u.curExp; u.maxExp; u.level; u.maxHp; u.power; }
{
	info.curExp += amount;
	while (info.curExp >= info.maxExp)
	{
		info.curExp -= info.maxExp;
		info.level++;
		info.maxExp += 5;

		int addHp = rand() % 5 + 1;
		int addPower = rand() % 5 + 1;

		info.maxHp += addHp;
		info.power += addPower;
	}
	this->Notify({UIEventType::EXP_Changed, info});
}

template<typename T>
template<typename U>
void InfoComponent<T>::SetSpeed(decltype(U::speed) amount) requires requires(U u) { u.speed; }
{
	info.speed = amount;
	this->Notify({UIEventType::Speed_Changed, info});
}

template<typename T>
template<typename U>
inline void InfoComponent<T>::SetGold(decltype(U::gold) amount) requires requires(U u) { u.gold; }
{
	if (amount < 0) amount = 0;
	info.gold = amount;
}

template<typename T>
template<typename U>
inline void InfoComponent<T>::AddGold(decltype(U::gold) amount) requires requires(U u) { u.gold; }
{
	SetGold(info.gold + amount);
}

template<typename T>
void InfoComponent<T>::Update(float dt)
{
	if constexpr (requires(T x) { x.curHp; })
	{
		if (!wasDead && info.curHp <= static_cast<decltype(info.curHp)>(0))
		{
			if (onZeroHpCallback)
				onZeroHpCallback();
			wasDead = true;
		}
		else if (info.curHp > static_cast<decltype(info.curHp)>(0))
		{
			wasDead = false;
		}
	}
}