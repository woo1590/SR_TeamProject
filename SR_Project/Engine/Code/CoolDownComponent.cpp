#include "EnginePCH.h"
#include "CoolDownComponent.h"
#include "Object.h"
#include "UIRenderer.h"

CoolDownComponent* CoolDownComponent::Create(Object* owner)
{
	auto* instance = new CoolDownComponent(owner);

	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT CoolDownComponent::Ready_Component()
{
    whiteOverlay = owner->AddComponent<UIRenderer>();
    whiteOverlay->SetTexture(L"white");
    whiteOverlay->SetLayer(10); 
    whiteOverlay->SetVisible(false);

    cooldownBar = owner->AddComponent<UIRenderer>();
    cooldownBar->SetTexture(L"white_bar");
    cooldownBar->SetLayer(11);
    cooldownBar->SetVisible(false);
	return S_OK;
}

void CoolDownComponent::Play()
{
    curTime = 0.f;
    playing = true;

    whiteOverlay->SetVisible(true);
    cooldownBar->SetVisible(true);
    whiteOverlay->SetAlpha(1.f);
}

void CoolDownComponent::Update(float dt)
{
    if (!playing) return;

    curTime += dt;
    float t = curTime / maxTime;
    t = clamp(t, 0.f, 1.f);

    whiteOverlay->SetAlpha(1.f - t);

    cooldownBar->ApplyRatioVertical(1.f - t); 

    if (t >= 1.f)
    {
        playing = false;
        whiteOverlay->SetVisible(false);
        cooldownBar->SetVisible(false);
    }
}