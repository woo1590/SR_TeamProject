#include "pch.h"
#include "BossIcon.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"

BossIcon* BossIcon::Create(ObjectManager* owner)
{
	auto* instance = new BossIcon(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT BossIcon::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto font = AddComponent<FontComponent>();

	transform->SetScale(0.3f, 0.5f);
	transform->SetPosition(200.f, -40.f);

	renderer->SetTexture(L"boss_icon");

	return S_OK;
}

void BossIcon::Update(float dt)
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

	font->AddText(L"·¹µå½ºÅæ °ñ·¥", {530, 40, 730, 150},Color::Red,DT_CENTER,FontType::QuestTitle);
}