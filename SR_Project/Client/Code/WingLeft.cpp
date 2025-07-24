#include "pch.h"
#include "WingLeft.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"
#include "IntroAnimComponent.h"
#include "scene.h"
#include "ObjectManager.h"

WingLeft* WingLeft::Create(ObjectManager* owner)
{
	auto instance = new WingLeft(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT WingLeft::Ready_Object()
{
	auto tf = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto font = AddComponent<FontComponent>();

	auto anim = AddComponent<IntroAnimComponent>();
	
	tf->SetScale(0.5f, 0.5f);
	tf->SetPosition(WINCX * 0.5f - 150, 30.f);
	renderer->SetTexture(L"wingleft");
	renderer->SetPivot(UIPivot::Top);
	renderer->SetAlpha(0.8f);

	font->SetAlpha(0.f);

	return S_OK;
}

void WingLeft::Update(float dt)
{
	Object::Update(dt);

	auto font = GetComponent<FontComponent>();
	
	if (state != AnimState::Finished)
	{
		animTimer += dt;
		switch (state)
		{
		case AnimState::FadingIn:
		{
			float alpha = min(animTimer / fadeInDur, 1.f);
			font->SetAlpha(alpha);
			if (alpha >= 1.f)
			{
				state = AnimState::Holding;
				animTimer = 0.f;
			}
			break;
		}

		case AnimState::Holding:
		{
			if (animTimer >= holdDur)
			{
				state = AnimState::FadingOut;
				animTimer = 0.f;
			}
			break;
		}

		case AnimState::FadingOut:
		{
			float alpha = 1.f - min(animTimer / fadeOutDur, 1.f);
			font->SetAlpha(alpha);
			if (alpha <= 0.f)
				state = AnimState::Finished;
			break;
		}
		}
	}
	
	font->ClearText();
	if (state != AnimState::Finished)
	{
		auto sceneID = owner->GetOwner()->GetSceneID();
		wstring text = (sceneID == SceneID::STAGE1) ? L"ÇÏ´Ã¼¶" : (sceneID == SceneID::STAGE2) ? L"¿£´õ ¿ùµå" : L"¸¶À»";
		font->AddText(text, {540, 100, 740, 200}, Color::White, DT_CENTER, FontType::IntroText);
	}
}
