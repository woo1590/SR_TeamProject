#include "pch.h"
#include "LevelUpEffect.h"


LevelUpEffect* LevelUpEffect::Create(ObjectManager* owner)
{
	auto instance = new LevelUpEffect(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT LevelUpEffect::Ready_Object()
{
	transform = AddComponent<TransformComponent>();
	renderer = AddComponent<UIRenderer>();
	font = AddComponent<FontComponent>();

	transform->SetPosition(WINCX * 0.5f, WINCY * 0.5f - 100);

	renderer->SetTexture(L"levelup");

	font->AddText(L"·¹º§ ¾÷", {0,0,1280,520}, Color::Yellow, 
		DT_CENTER | DT_VCENTER, FontType::DeathCount);
	font->SetVisible(false);

	return S_OK;
}

void LevelUpEffect::Update(float dt)
{
	Object::Update(dt);

	constexpr float popDur     = 0.2f;
	constexpr float settleDur  = 0.4f;
	constexpr float holdDur    = 2.f;
	constexpr float fadeOutDur = 0.3f;

	const float totalDur = popDur + settleDur + holdDur + fadeOutDur;

	elapsedTime += dt;
	if (elapsedTime >= totalDur)
	{
		this->SetDead();
		return;
	}
	float scale = 2.5f;
	float alpha = 1.f;

	auto easeOutCubic = [](float x) -> float {return 1.f - pow(1.f - x, 3); };

	if (elapsedTime < popDur)
	{
		float progress = elapsedTime / popDur;
		scale = lerp(1.5f, 3.f, easeOutCubic(progress));
		alpha = lerp(0.f, 1.f, progress);
	}
	else if (elapsedTime < popDur + settleDur)
	{
		float progress = (elapsedTime - popDur) / settleDur;
		scale = lerp(3.5f, 2.5f, progress);
	}
	else if (elapsedTime < popDur + settleDur + holdDur)
	{
		scale = 2.5f;
		alpha = 1.f;
	}
	else
	{
		float progress = (elapsedTime - (popDur + settleDur + holdDur)) / fadeOutDur;
		scale = 2.5f;
		alpha = lerp(1.f, 0.f, progress);
	}
	transform->SetScale(scale, scale);
	font->SetAlpha(alpha);
	renderer->SetAlpha(alpha);
}
