#include "pch.h"
#include "HP_Potion.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "Player.h"
#include "ObjectManager.h"


HP_Potion* HP_Potion::Create(ObjectManager* owner)
{
	auto* instance = new HP_Potion(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HP_Potion::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto hover = AddComponent<HoverComponent>();

	transform->SetPosition(700.f, 700.f);
	renderer->SetScale(0.8f, 0.8f);
	renderer->SetTexture(L"hp_potion");

	hover->SetUpdateCallBack([this](bool isHovered)
		{
			if (!isHovered) return;

			auto input = EngineCore::GetInstance()->GetInputSystem();
			if (input->IsKeyPressed(KEY::LBUTTON))
			{
				auto player = owner->GetFrontObject(ObjectType::Player);
				auto playerInfo = player->GetComponent<InfoComponent<PlayerInfo>>();

				playerInfo->AddHp(10);
			}
		});

	return S_OK;
}
