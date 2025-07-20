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
	void AddExp(int amount) requires requires(U u) { u.curExp; u.maxExp; u.level; };

	template<typename U = T>
	void SetSpeed(decltype(U::speed) amount) requires requires(U u) { u.speed; };

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
void InfoComponent<T>::AddExp(int amount) requires requires(U u) { u.curExp; u.maxExp; u.level; }
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

template<typename T>
template<typename U>
void InfoComponent<T>::SetSpeed(decltype(U::speed) amount) requires requires(U u) { u.speed; }
{
	info.speed = amount;
	this->Notify({UIEventType::Speed_Changed, info});
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

// U를 사용해서 "템플릿 함수"로 만드는 것 자체가 목적.
// 일반 멤버 함수를 템플릿 함수로 "신분 상승" 시키는 순간, 컴파일러가 제약 조건을 처리하는 방식이 근본적으로 달라진다.
 
// 확정된 기능이 아니라, 나중에 필요하면 만들수 있는 옵션이구나 라고컴파일러가 판단.
// 나중에 코드 어딘가에서 호출하려는 시도가 있을때, 먼저 가능한지 조건 부터 확인한다. 
// 만약 없으면, 이번 경우에는 실행할수 없다고 판단하고 조용히 무시한다 (SFINAE) 규칙.
// 
// 클래스템플릿 -> 함수 템플릿 (필수 -> 선택 사양으로 바꿈)