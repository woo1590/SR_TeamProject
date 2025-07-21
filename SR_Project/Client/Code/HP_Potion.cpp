#include "pch.h"
#include "HP_Potion.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "ItemComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "ObjectManager.h"
#include "CoolDownComponent.h"
#include "MaskObj.h"


HP_Potion* HP_Potion::Create(ObjectManager* owner)
{
	auto* instance = new HP_Potion(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HP_Potion::Ready_Object()
{
	auto transform  = AddComponent<TransformComponent>();
	auto renderer   = AddComponent<UIRenderer>();
	auto hover      = AddComponent<HoverComponent>();
	auto info       = AddComponent<InfoComponent<ItemInfo>>();
	auto item       = AddComponent<ItemComponent>();
	auto cooldown   = AddComponent<CoolDownComponent>();

	transform->SetPosition(700.f, 700.f);
	transform->SetScale(0.8f, 0.8f);
	renderer->SetTexture(L"hp_potion");

	auto maskObj = MaskObj::Create(owner);
	auto maskTf = maskObj->GetComponent<TransformComponent>();

	auto maskRenderer = maskObj->GetComponent<UIRenderer>();

	info->SetInfo({L"HP 포션", L"hp_potion", ItemType::Potion, Rarity::Default, 10, L"HP 10 회복"});
	item->SetCoolDown(true, 5.f);

	cooldown->Init(item, maskRenderer);

	hover->SetUpdateCallBack([this](bool isHovered) {
		auto input = EngineCore::GetInstance()->GetInputSystem();

		const bool leftClick = isHovered && input->IsKeyPressed(KEY::LBUTTON);
		const bool keyE = input->IsKeyPressed(KEY::E);

		if (leftClick || keyE)
		{
			auto player = owner->GetFrontObject(ObjectType::Player);
			auto itemComponent = GetComponent<ItemComponent>();
			if (itemComponent && player)
				itemComponent->Use(player);
		}
		});

	owner->AddUIObject(maskObj);
	return S_OK;
}
