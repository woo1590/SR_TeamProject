#include "EnginePCH.h"
#include "IntroAnimComponent.h"
#include "UIRenderer.h"
#include "Object.h"

IntroAnimComponent* IntroAnimComponent::Create(Object* owner)
{
	auto instance = new IntroAnimComponent(owner);
	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT IntroAnimComponent::Ready_Component()
{
	renderer = owner->GetComponent<UIRenderer>();
	assert(renderer && "IntroAnimComponent needs UIRenderer");

	renderer->ApplyRatioVerticalFromTop(0.f);

	return S_OK;
}

void IntroAnimComponent::Update(float dt)
{
	if (state == AnimState::Finished) return;

	animTimer += dt;

	switch (state)
	{
	case AnimState::AnimatingIn:
	{
		float ratio = min(animTimer / fadeInDur, 1.f);
		renderer->ApplyRatioVerticalFromTop(ratio);
		if (ratio >= 1.f)
		{
			state = AnimState::Holding;
			animTimer = 0.f;
		}
		break;
	}

	case AnimState::Holding:
	{
		if (animTimer >= holdDur)
		{
			renderer->SetVisible(false);
			state = AnimState::FadingOut;
		}
		break;
	}

	case AnimState::FadingOut:
	{
		float alpha = 1.f - min(animTimer / fadeOutDur, 1.f);
		renderer->SetAlpha(alpha);

		if (alpha <= 0.f)
		{
			renderer->SetVisible(false);
			state = AnimState::Finished;
		}
	}
	}
}
