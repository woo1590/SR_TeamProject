#pragma once

#include "ObjectComponent.h"
#include "IObserver.h"
#include "Object.h"
#include "Subject.h"
#include "UIRenderer.h"

BEGIN(Engine)

class SliderComponent final : public ObjectComponent, public Subject, public IObserver
{
public:
	explicit SliderComponent(Object* owner)
		:ObjectComponent(owner)
	{
		front = owner->AddComponent<UIRenderer>();
		mid   = owner->AddComponent<UIRenderer>();
		back  = owner->AddComponent<UIRenderer>();
		
		front->SetTexture(L"hpbar_front");
		mid  ->SetTexture(L"hpbar_mid");
		back ->SetTexture(L"hpbar_back");
	}

private:
	int maxHp = 1, curHp = 1;
	float targetRatio = 1.f, frontRatio = 1.f, midRatio = 1.f;
	const float speed = 4.f;

	UIRenderer* back = nullptr;
	UIRenderer* mid = nullptr;
	UIRenderer* front = nullptr;


};

END