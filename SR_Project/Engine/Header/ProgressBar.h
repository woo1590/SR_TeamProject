#pragma once

#include "ObjectComponent.h"
#include "IObserver.h"
#include "UIRenderer.h"
#include "Object.h"

BEGIN(Engine)

template<typename T>
class ProgressBar: public ObjectComponent, public IObserver<UIEvent<T>>
{
protected:
	explicit ProgressBar(Object* owner) :ObjectComponent(owner) {}

public:
	static ProgressBar* Create(Object* owner);
	HRESULT Ready_Component();
	
	void SetUIEventType(UIEventType type) { eventType = type; }
	void SetBarDirection(BarDirection _dir) { barDir = _dir; }

	void Update(float dt) override;
	void OnNotify(const UIEvent<T>& event) override;

private:
	void ApplyRatio(float ratio);

protected:
	UIRenderer* renderer = nullptr;

	int curValue = 0;
	int maxValue = 1;

	float targetRatio = 1.f;
	float curRatio = 0.f;
	const float speed = 8.f;

	UIEventType eventType = UIEventType::HP_Changed;
	BarDirection barDir = BarDirection::Vertical;
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
	targetRatio = static_cast<float>(curValue) / max(1, maxValue);
}

template<typename T>
void ProgressBar<T>::Update(float dt)
{
	if (fabs(curRatio - targetRatio) > 0.01f)
	{
		float t = clamp(dt * speed, 0.f, 1.f);
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
