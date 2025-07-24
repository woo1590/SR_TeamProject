#include "pch.h"
#include "UIDebugObj.h"
#include "FontComponent.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"

#include "ResourceManager.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "Object.h"
#include "ObjectManager.h"
#include "LevelUpEffect.h"
#include "SoundManager.h"
#include "EngineCore.h"


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

	prevLevel = owner->GetFrontObject(ObjectType::Player)->
		GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().level;

	return S_OK;
}

void UIDebugObj::Update(float dt)
{
	const auto& playerInfo = owner->GetFrontObject(ObjectType::Player)->GetComponent<InfoComponent<PlayerInfo>>();
	const auto& info = playerInfo->GetInfo();
	auto font = GetComponent<FontComponent>();

	font->ClearText();

	RECT debugRect = {900, 300, 1400, 650};

	//font->AddText(
	//	L"Hp: " + to_wstring((int)info.curHp) + L"/" + to_wstring((int)info.maxHp) + 
	//	L"\nExp: " + to_wstring(info.curExp) + L"/" + to_wstring(info.maxExp) +
	//	L"\nspeed: " + to_wstring((int)info.speed),
	//	debugRect, Color::Cyan, DT_LEFT, FontType::Title);

	accTime += dt;
	++frameCount;

	if (accTime >= 1.f)
	{
		fps = static_cast<float>(frameCount) / accTime;
		accTime = 0.f;
		frameCount = 0;
	}

	//font->AddText(L"FPS: " + to_wstring((int)fps), {900, 250, 1250, 450}, Color::Cyan, DT_LEFT, FontType::Title);
	font->AddText(to_wstring((int)playerInfo->GetInfo().gold), 
		{1040,655,1150,700}, Color::White, DT_LEFT, FontType::CookieRunFont);

	const auto& input = EngineCore::GetInstance()->GetInputSystem();
	if (input->IsKeyPressed(KEY::ESC))
		exit(0);
	if (input->IsKeyPressed(KEY::L))
		playerInfo->AddExp(10);

	if (info.level > prevLevel)
	{
		prevLevel = info.level;
		EngineCore::GetInstance()->GetSoundManager()->PlaySFX("LevelUp");
		owner->AddUIObject(LevelUpEffect::Create(owner));
	}
}