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
	return S_OK;
}

void CoolDownComponent::Start(float _duration)
{
	duration = _duration;
	elapsed = 0.f;
	isCooling = true;

	auto renderer = owner->GetComponent<UIRenderer>();
	assert(renderer && "CoolDownComponent::Start - renderer missing");
	renderer->ApplyRatioVertical(1.f);
}

void CoolDownComponent::Update(float dt)
{
	if (!isCooling) return;

	elapsed += dt;
	float ratio = 1.f - (elapsed / duration);
	ratio = clamp(ratio, 0.f, 1.f);

	auto renderer = owner->GetComponent<UIRenderer>();
	renderer->ApplyRatioVertical(ratio);

	if (elapsed >= duration)
		isCooling = false;
}

