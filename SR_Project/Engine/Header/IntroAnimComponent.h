#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)
class UIRenderer;

class ENGINE_DLL IntroAnimComponent :public ObjectComponent
{
private:
	IntroAnimComponent(Object* owner) : ObjectComponent(owner) {}
	enum class AnimState {AnimatingIn, Holding,FadingOut, Finished};

public:
	static IntroAnimComponent* Create(Object* owner);
	HRESULT Ready_Component();

	void Update(float dt) override;

private:
	UIRenderer* renderer = nullptr;

	AnimState state = AnimState::AnimatingIn;
	float animTimer = 0.f;

	const float fadeInDur = 1.f;
	const float holdDur = 1.5f;
	const float fadeOutDur = 1.f;
};

END