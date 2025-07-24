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
#include "SoundManager.h"
#include "EngineCore.h"

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
	_vec3 pos = transform->GetPosition();

	auto maskObj = MaskObj::Create(owner);
	auto maskTf = maskObj->GetComponent<TransformComponent>();
	maskTf->SetPosition(pos.x, pos.y - 20.f);

	auto maskRenderer = maskObj->GetComponent<UIRenderer>();
	item->SetCoolDown(true, 5.f);
	item->SetItemType(ItemType::HpPotion);

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

	item->SetUseCallBack([=](Object* user) {
		if (auto hpInfo = user->GetComponent<InfoComponent<PlayerInfo>>())
		{
			hpInfo->AddHp(100);
			EngineCore::GetInstance()->GetSoundManager()->PlaySFX("UsePotion");
		}
		});

	owner->AddUIObject(maskObj);
	return S_OK;
}
