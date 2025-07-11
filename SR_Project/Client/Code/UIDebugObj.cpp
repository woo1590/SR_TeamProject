#include "pch.h"
#include "UIDebugObj.h"
#include "FontComponent.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "AIController.h"

#include "ResourceManager.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Player.h"


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

	return S_OK;
}

void UIDebugObj::Update(float dt)
{
	const auto& playerInfo = owner->GetFrontObject(ObjectType::Player)->GetComponent<InfoComponent<PlayerInfo>>();
	const auto& info = playerInfo->GetInfo();
	auto font = GetComponent<FontComponent>();

	font->ClearText();

	RECT debugRect = {900, 300, 1400, 650};

	font->AddText(
		L"Hp: " + to_wstring(info.curHp) + L"/" + to_wstring(info.maxHp) + 
		L"\nExp: " + to_wstring(info.curExp) + L"/" + to_wstring(info.maxExp) +
		L"\nspeed: " + to_wstring((int)info.speed),
		debugRect, Color::Cyan, DT_LEFT, FontType::Title);

	accTime += dt;
	++frameCount;

	if (accTime >= 1.f)
	{
		fps = static_cast<float>(frameCount) / accTime;
		accTime = 0.f;
		frameCount = 0;
	}

	font->AddText(L"FPS: " + to_wstring((int)fps), { 900,250,1250,450 }, Color::Cyan,DT_LEFT, FontType::Title);

	const auto& input = EngineCore::GetInstance()->GetInputSystem();
	if (input->IsKeyPressed(KEY::ESC))
		exit(0);
}