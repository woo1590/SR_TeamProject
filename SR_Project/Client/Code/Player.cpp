#include "pch.h"
#include "Player.h"
#include "PlayerInfoComponent.h"
#include "FontComponent.h"
#include "InputSystem.h"
#include "EngineCore.h"
#include "UIRenderer.h"
#include "TransformComponent.h"

Player* Player::Create(ObjectManager* owner, ObjectType type)
{
	auto* instance = new Player(owner, type);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT Player::Ready_Object()
{
	auto info = AddComponent<PlayerInfoComponent>();
	auto font = AddComponent<FontComponent>();
	auto renderer = AddComponent<UIRenderer>();

	font->SetRect({50, 200, 500, 500});
	font->SetColor(Color::Cyan);
	font->SetFontType(FontType::Title);

	return S_OK;
}

void Player::Update(float dt)
{
	const auto& input = EngineCore::GetInstance()->GetInputSystem();

	const auto& info = GetComponent<PlayerInfoComponent>()->GetInfo();
	auto font = GetComponent<FontComponent>();

	if (input->IsKeyPressed(KEY::LBUTTON))
		GetComponent<PlayerInfoComponent>()->AddHp(-10);

	wchar_t buffer[128];
	swprintf_s(buffer, 128, L"Level: %d\ncurHp: %d\nmaxHp: %d\ncurExp: %d\nmaxExp:%d", info.level,info.curHp, info.maxHp, info.curExp, info.maxExp);

	font->SetText(buffer);
}
