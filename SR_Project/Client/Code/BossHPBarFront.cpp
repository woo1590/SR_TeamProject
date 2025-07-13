#include "pch.h"
#include "BossHPBarFront.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ProgressBar.h"
#include "FontComponent.h"

BossHPBarFront* BossHPBarFront::Create(ObjectManager* owner)
{
	auto* instance = new BossHPBarFront(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT BossHPBarFront::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto hpFront = AddComponent<ProgressBar<EnemyInfo>>();
	auto font = AddComponent<FontComponent>();

	transform->SetPosition(300.f, 100.f);
	transform->SetScale(0.3f, 0.4f);

	renderer->SetPivot(UIPivot::Left);
	renderer->SetTexture(L"boss_hpbarfront");
	renderer->SetAlpha(0.8f);

	hpFront->SetBarDirection(BarDirection::Horizontal);
	hpFront->SetEventType(UIEventType::HP_Changed);

	return S_OK;
}

void BossHPBarFront::Update(float dt)
{
	Object::Update(dt);

	elapsedTime += dt;

	auto font = GetComponent<FontComponent>();
	font->ClearText();

	float alpha = 1.f;
	if (!fadeCompleted)
	{
		alpha = elapsedTime / fadeDuration;
		if (alpha >= 1.f)
		{
			alpha = 1.f;
			fadeCompleted = true;
		}
		font->SetAlpha(alpha);
	}

	font->AddText(L"·¹µå½ºÅæ °ñ·¥", {530, 40, 730, 150}, Color::Red, DT_CENTER, FontType::QuestTitle);
}