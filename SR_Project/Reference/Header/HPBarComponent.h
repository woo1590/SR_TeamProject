#pragma once

#include "ObjectComponent.h"
#include "IObserver.h"
#include "Object.h"
#include "SliderComponent.h"

// OnNotify()로 HP 변화 이벤트를 수신함
// 내부적으로 SliderComponent를 AddComponent()로 추가해서 SetRatio() 를 호출.

BEGIN(Engine)

class ENGINE_DLL HPBarComponent : public ObjectComponent, public IObserver
{
public:
	HPBarComponent(Object* owner)
		:ObjectComponent(owner)
	{
		slider = owner->AddComponent<SliderComponent>();
	}

	static HPBarComponent* Create(Object* owner);

	void OnNotify(const NotifyEvent& event) override;

private:
	SliderComponent* slider = nullptr;
};

END