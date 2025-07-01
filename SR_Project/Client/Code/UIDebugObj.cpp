#include "pch.h"
#include "UIDebugObj.h"
#include "FontComponent.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

#include "PlayerInfoComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"

UIDebugObj* UIDebugObj::Create(ObjectManager* owner)
{
	auto* instance = new UIDebugObj(owner);

	return FAILED(instance->Ready_Object()) ? Safe_Release(instance), nullptr : instance;
}

HRESULT UIDebugObj::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto font      = AddComponent<FontComponent>();

	font->SetColor(Color::Cyan);
	font->SetFontType(FontType::Title);
	font->SetRect({50, 50, 500, 500});

	return S_OK;
}

void UIDebugObj::Update(float dt)
{
	auto font = GetComponent<FontComponent>();
	const auto& info = player->GetComponent<PlayerInfoComponent>()->GetInfo();

	accTime += dt;
	++frameCount;

	if (accTime >= 1.f)
	{
		fps = static_cast<float>(frameCount) / accTime;
		accTime = 0.f;
		frameCount = 0;
	}
	
	wchar_t buffer[128];
	swprintf_s(buffer, L"FPS: %.f\nLevel: %d\ncurHp: %d\nmaxHp: %d\ncurExp: %d/maxExp: %d\nspeed: %.1f",
		fps, info.level, info.curHp, info.maxHp, info.curExp, info.maxExp, info.speed);
	
	font->SetText(buffer);

	const auto& input = EngineCore::GetInstance()->GetInputSystem();
	auto comp = player->GetComponent<PlayerInfoComponent>();
	if (input->IsKeyPressed(KEY::LBUTTON))
	{
		comp->AddHp(-10);
		comp->AddExp(5);
	}
}
