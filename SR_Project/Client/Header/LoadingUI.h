#pragma once

#include "Object.h"

class LoadingUI : public Object
{
private:
	LoadingUI(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static LoadingUI* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;
	void SetVisible(bool _visible) { visible = _visible; }
	
private:
	float pulseTime = 0.f;
	bool visible = true;

	static constexpr float fadeDur = 2.f;
	//static constexpr float scaleTimer = 0.f;
};

