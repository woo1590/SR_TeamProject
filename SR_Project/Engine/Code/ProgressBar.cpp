#include "EnginePCH.h"
#include "ProgressBar.h"
#include "Object.h"

ProgressBar* ProgressBar::Create(Object* owner)
{
    auto* instance = new ProgressBar(owner);
    
    return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ProgressBar::Ready_Component()
{
    renderer = owner->GetComponent<UIRenderer>();

    assert(renderer && "ProgressBarComponent::Ready_Component - UIRenderer not found");

    return S_OK;
}

void ProgressBar::OnNotify(const NotifyEvent& event)
{
    if (event.type != static_cast<int>(notifyType))
        return;

    assert(event.data && "OnNotify - event.data is null");

    int cur = 0, max = 1;
    ExtractValues(event.data, cur, max);

    curValue = cur;
    maxValue = max;

    targetRatio = static_cast<float>(curValue) / max(1, maxValue);
}

void ProgressBar::Update(float dt)
{
    if (fabs(curRatio - targetRatio) > 0.01f)
    {
        float t = clamp(dt * speed, 0.f, 1.f);
        curRatio = curRatio + (targetRatio - curRatio) * t;
        ApplyRatio(curRatio);
    }
}

void ProgressBar::ApplyRatio(float ratio)
{
    assert(renderer && "ApplyRatio - renderer is null");
   
    if (barDir == BarDirection::Vertical)
        renderer->ApplyRatio(ratio);
    else
        renderer->ApplyRatioHorizontal(ratio);
}