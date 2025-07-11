#include "EnginePCH.h"
#include "ParticleComponent.h"
#include "Object.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

ParticleComponent* ParticleComponent::Create(Object* owner)
{
	auto* instance = new ParticleComponent(owner);

	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ParticleComponent::Ready_Component()
{
	if (!owner->GetComponent<UIRenderer>())
	{
		auto renderer = owner->AddComponent<UIRenderer>();
		renderer->SetVisible(true);
		renderer->SetAlpha(1.f);
		renderer->SetTexture(L"debugui");
	}
	return S_OK;
}

void ParticleComponent::SetSizeRange(float start, float end)
{
	startSize = start;
	endSize = end;
}

void ParticleComponent::Update(float dt)
{
	elapsed += dt;
	if (elapsed > life)
	{
		owner->SetDead();
		return;
	}

	float t = elapsed / life;
	float size = lerp(startSize, endSize, t);
	float alpha = glow ? sinf(t * 3.14f) : 1.f - t;

	if (auto tf = owner->GetComponent<TransformComponent>())
		tf->SetScale(size, size);
	
	if (auto renderer = owner->GetComponent<UIRenderer>())
	{
		renderer->SetAlpha(alpha);
		renderer->SetVisible(true);
	}
}

void ParticleComponent::Render()
{

}