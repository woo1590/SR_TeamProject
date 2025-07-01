#pragma once

#include "ObjectComponent.h"
#include "IObserver.h"
#include "UIRenderer.h"

BEGIN(Engine)

class ENGINE_DLL ProgressBarComponent: public ObjectComponent, public IObserver
{
protected:
	explicit ProgressBarComponent(Object* owner)
		:ObjectComponent(owner) {
	}

public:
	ProgressBarComponent* Create(Object* owner);

	HRESULT Ready_Component();
	void SetNotifyType(NotifyType type) { notifyType = type; }

	void OnNotify(const NotifyEvent& event) override;
	void Update(float dt) override;

	void SetBarDirection(BarDirection _dir) { barDir = _dir; }

protected:
	virtual void ExtractValues(const void* data, int& cur, int& max) {}

private:
	void ApplyRatio(float ratio);

protected:
	UIRenderer* renderer = nullptr;

	int curValue = 0;
	int maxValue = 1;

	float targetRatio = 1.f;
	float curRatio = 1.f;
	const float speed = 8.f;

	NotifyType notifyType = NotifyType::None;
	BarDirection barDir = BarDirection::Vertical;
};

END