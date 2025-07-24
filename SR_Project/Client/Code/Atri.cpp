#include "pch.h"
#include "Atri.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

Atri* Atri::Create(ObjectManager* owner)
{
	auto* instance = new Atri(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT Atri::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetScale(targetScale.x, targetScale.y);
	transform->SetPosition(250.f, 420.f);

	renderer->SetRenderType(UIRenderType::QuestUI);
	renderer->SetTexture(L"6");

	return S_OK;
}

void Atri::SetEmotion(Emotion emotion)
{
	auto renderer = GetComponent<UIRenderer>();
	auto tf = GetComponent<TransformComponent>();

	if (!renderer) return;

	renderer->SetTexture(to_wstring(static_cast<int>(emotion)));

	isAnimating = true;
	animTimer = 0.f;
	tf->SetScale(startScale.x,targetScale.y);
}

void Atri::Update(float dt)
{
	if (!isAnimating) return;

	animTimer += dt;
	float progress = animTimer / animDur;
	
	if (progress >= 1.f)
	{
		progress = 1.f;
		isAnimating = false;
	}

	float scaleX = lerp(startScale.x, targetScale.x, progress);
	float scaleY = lerp(startScale.y, targetScale.y, progress);

	GetComponent<TransformComponent>()->SetScale(scaleX,scaleY);
}