#include "pch.h"
#include "DeathFrame.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"
#include "ObjectManager.h"
#include "Player.h"

DeathFrame* DeathFrame::Create(ObjectManager* owner)
{
	auto* instance = new DeathFrame(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT DeathFrame::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto font      = AddComponent<FontComponent>();
	
	transform->SetPosition(640.f, 250.f);
	transform->SetScale(0.25f,0.25f);
	
	renderer->SetTexture(L"deathframe");
	renderer->SetRenderType(UIRenderType::DeathUI);

	return S_OK;
}

void DeathFrame::Update(float dt)
{
	auto renderer = GetComponent<UIRenderer>();
	UIRenderType curType = renderer->GetCurRenderType();
	UIRenderType myType = renderer->GetRenderType();
	
	if (curType != prevRenderType)
	{
		if (curType == myType)
			Reset();
		prevRenderType = curType;
	}

	if (curType != myType) return;

	Object::Update(dt);

	fadeElapsed += dt;
	
	if (!appeared && fadeElapsed >= appearDelay)
	{
		appeared = true;
		fadeElapsed = 0.f;
	}

	auto font = GetComponent<FontComponent>();
	font->ClearText();

	if (!appeared)
		return;

	float alpha = clamp(fadeElapsed / fadeDuration, 0.f, 1.f);
	font->SetAlpha(alpha);

	float t = clamp(fadeElapsed / fadeDuration, 0.f, 1.f);
	float scale = 0.2f + t * 0.8f;

	RECT deathRect = {
		(int)(640 - 150 * scale),
		(int)(450 - 75  * scale),
		(int)(640 + 150 * scale),
		(int)(450 + 75  * scale)
	};
	
	font->AddText(L"사망했습니다", deathRect,
		Color::White, DT_CENTER | DT_VCENTER, FontType::DeathText);

	countElapsed += dt;
	if (countElapsed >= 1.f && deathCount > 0)
	{
		countElapsed = 0.f;
		deathCount--;

		if (deathCount <= 0)
			destroyAfterCount = true;
	}

	font->AddText(to_wstring(deathCount), {650, 500, 700, 650},
		Color::White, DT_CENTER | DT_VCENTER, FontType::DeathCount);

	font->AddText(L"시작까지       초", {470, 500, 800, 650},
		Color::White, DT_CENTER | DT_VCENTER, FontType::QuestTitle);

	if (destroyAfterCount)
	{
		renderer->SetCurRenderType(UIRenderType::MainGame);
		Player* player = dynamic_cast<Player*>(owner->GetFrontObject(ObjectType::Player));
		if (player)
			player->RevivePlayer();
	}
}

void DeathFrame::Reset()
{
	deathCount = 3;
	fadeElapsed = 0.f;
	countElapsed = 0.f;
	destroyAfterCount = false;
	appeared = false;
}