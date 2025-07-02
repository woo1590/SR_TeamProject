#include "pch.h"
#include "UIDebugObj.h"
#include "FontComponent.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"

#include "ResourceManager.h"
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

	font->SetFontType(FontType::Title);

	return S_OK;
}

void UIDebugObj::Update(float dt)
{
	const auto& playerInfo = player->GetComponent<InfoComponent<PlayerInfo>>();
	const auto& info = playerInfo->GetInfo();
	auto font = GetComponent<FontComponent>();

	font->ClearText();
	font->AddText(L"Lv: " + to_wstring(info.level), {600, 650, 800, 770}, Color::Cyan);

	RECT debugRect = {1000, 400, 1400, 650};

	font->AddText(
		L"Hp: " + to_wstring(info.curHp) + L"/" + to_wstring(info.maxHp) + 
		L"\nExp: " + to_wstring(info.curExp) + L"/" + to_wstring(info.maxExp) +
		L"\nspeed: " + to_wstring((int)info.speed),
		debugRect, Color::Cyan);

	accTime += dt;
	++frameCount;

	if (accTime >= 1.f)
	{
		fps = static_cast<float>(frameCount) / accTime;
		accTime = 0.f;
		frameCount = 0;
	}

	font->AddText(L"Lv: " + to_wstring((int)fps), { 650,450,850,650 }, Color::Cyan);
	
	const auto& input = EngineCore::GetInstance()->GetInputSystem();
	if (input->IsKeyPressed(KEY::LBUTTON))
	{
	//	playerInfo->AddExp(5);
	//	playerInfo->AddHp(-10);
	}
}
