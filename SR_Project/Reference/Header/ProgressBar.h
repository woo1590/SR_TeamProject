#pragma once

#include "ObjectComponent.h"
#include "IObserver.h"
#include "UIRenderer.h"
#include "Object.h"
#include "InfoComponent.h"
#include "Subject.h"

enum class RenderPolicy { Always, HideWhenFull };

BEGIN(Engine)

template<typename T>
class ProgressBar : public ObjectComponent, public IObserver<UIEvent<T>>
{
protected:
	explicit ProgressBar(Object* owner) :ObjectComponent(owner) {}

public:
	static ProgressBar* Create(Object* owner);
	HRESULT Ready_Component();

	void SetEventType(UIEventType type) { eventType = type; }
	void SetBarDirection(BarDirection _dir) { barDir = _dir; }
	void SetRenderPolicy(RenderPolicy policy) { renderPolicy = policy; }

	void Update(float dt) override;
	void OnNotify(const UIEvent<T>& event) override;

	void AppearAnimation(float duration = 1.f);

	void SetLerpSpeed(float _speed) { lerpSpeed = max(0.f, _speed); }
	void SetDelay(float _delay) { delay = max(0.f, _delay); }

private:
	void ApplyRatio(float ratio);

protected:
	UIRenderer* renderer = nullptr;

	int curValue = 0;
	int maxValue = 1;

	float targetRatio = 1.f;
	float curRatio = 1.f;

	UIEventType eventType = UIEventType::HP_Changed;
	BarDirection barDir = BarDirection::Vertical;
	RenderPolicy renderPolicy = RenderPolicy::Always;

	bool isAppearing = false;
	float appearElapsed = 0.f;
	float appearDuration = 1.f;
	float actualTargetRatio = 1.f;

	float lerpSpeed = 8.f;
	float delay = 0.f;
	float delayLeft = 0.f;
};

END

template<typename T>
ProgressBar<T>* ProgressBar<T>::Create(Object* owner)
{
	auto* instance = new ProgressBar<T>(owner);
	return FAILED(instance->Ready_Component()) ? Safe_Release(instance), nullptr : instance;
}

template<typename T>
inline HRESULT ProgressBar<T>::Ready_Component()
{
	renderer = owner->GetComponent<UIRenderer>();
	assert(renderer && "ProgressBar::Ready_Component - UIRenderer is null");
	return S_OK;
}

template<typename T>
void ProgressBar<T>::OnNotify(const UIEvent<T>& event)
{
	if (event.type != eventType)
		return;

	switch (event.type)
	{
	case UIEventType::HP_Changed:
		curValue = event.info.curHp;
		maxValue = event.info.maxHp;
		break;

	case UIEventType::EXP_Changed:
		curValue = event.info.curExp;
		maxValue = event.info.maxExp;
		break;

	default:
		return;
	}

	assert(maxValue > 0 && "ProgressBar: maxValue must be > 0");
	actualTargetRatio = static_cast<float>(curValue) / max(1, maxValue);

	if (!isAppearing)
		targetRatio = actualTargetRatio;

	if (event.type == UIEventType::HP_Changed)
	{
		if (actualTargetRatio < curRatio)
			delayLeft = delay;
		else
			delayLeft = 0.f, targetRatio = actualTargetRatio;
	}
}

template<typename T>
inline void ProgressBar<T>::AppearAnimation(float duration)
{
	isAppearing = true;
	appearElapsed = 0.f;
	appearDuration = duration;
	targetRatio = 0.f;
	curRatio = 0.f;
}

template<typename T>
void ProgressBar<T>::Update(float dt)
{
	const bool hide = (renderPolicy == RenderPolicy::HideWhenFull && actualTargetRatio >= 0.99f);

	renderer->SetVisible(!hide);

	for (auto* child : owner->GetChildren())
	{
		if (auto* r = child->GetComponent<UIRenderer>())
			r->SetVisible(!hide);
	}


	if (isAppearing)
	{
		appearElapsed += dt;
		float t = clamp(appearElapsed / appearDuration, 0.f, 1.f);
		targetRatio = actualTargetRatio * t;

		if (t >= 1.f)
			isAppearing = false;
	}

	if (delayLeft > 0.f)
	{
		delayLeft = max(0.f, delayLeft - dt);
		if (delayLeft <= 0.f)
			targetRatio = actualTargetRatio;
	}

	if (fabs(curRatio - targetRatio) > 0.01f)
	{
		float t = clamp(dt * lerpSpeed, 0.f, 1.f);
		curRatio += (targetRatio - curRatio) * t;
		ApplyRatio(curRatio);
	}
}

template<typename T>
void ProgressBar<T>::ApplyRatio(float ratio)
{
	assert(renderer && "ProgressBar::ApplyRatio - renderer is null");

	switch (barDir)
	{
	case BarDirection::Vertical:
		renderer->ApplyRatioVertical(ratio);
		break;

	case BarDirection::Horizontal:
		renderer->ApplyRatioHorizontal(ratio);
		break;

	case BarDirection::ClockWise16:

		break;
	}
}