#include "pch.h"
#include "BoostItem.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"
#include "CooldownComponent.h"
#include "MaskObj.h"
#include "ObjectManager.h"
#include "Player.h"
#include "EngineCore.h"
#include "SoundManager.h"

BoostItem* BoostItem::Create(ObjectManager* owner)
{
	auto* instance = new BoostItem(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT BoostItem::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.35f, 0.35f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetTexture(L"inventory_boost");
	renderer->SetRenderType(UIRenderType::Inventory);

	auto info = AddComponent<InfoComponent<ItemInfo>>();
	auto item = AddComponent<ItemComponent>();
	auto cooldown = AddComponent<CoolDownComponent>();

	item->SetItemType(ItemType::BoostItem);
	item->SetOriginalScale({0.35f, 0.35f});

	auto maskObj = MaskObj::Create(owner);
	auto maskTf = maskObj->GetComponent<TransformComponent>();
	maskTf->SetParent(transform);
	maskTf->SetPosition(0, 30.f);

	auto maskRenderer = maskObj->GetComponent<UIRenderer>();
	item->SetCoolDown(true, 5.f);

	cooldown->Init(item, maskRenderer);

	item->SetUseCallBack([=](Object* user) {
		auto playerInfo = user->GetComponent<InfoComponent<PlayerInfo>>();
		EngineCore::GetInstance()->GetSoundManager()->PlaySFX("UseBoost");
		auto info = playerInfo->GetInfo();
		info.speed = 25.f;
		playerInfo->SetInfo(info);
		});
	cooldown->SetCompleteCallback([=]() {
		auto playerInfo = owner->GetFrontObject(ObjectType::Player)->GetComponent<InfoComponent<PlayerInfo>>();
		auto info = playerInfo->GetInfo();
		info.speed = 15.f;
		playerInfo->SetInfo(info);
		});

	owner->AddUIObject(maskObj);
	return S_OK;
}