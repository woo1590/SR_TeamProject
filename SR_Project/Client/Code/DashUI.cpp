#include "pch.h"
#include "DashUI.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ItemComponent.h"
#include "CoolDownComponent.h"
#include "MaskObj.h"
#include "Player.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "ObjectManager.h"

DashUI* DashUI::Create(ObjectManager* owner)
{
	auto* instance = new DashUI(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT DashUI::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto info = AddComponent<InfoComponent<ItemInfo>>();
	auto renderer = AddComponent<UIRenderer>();
	auto item = AddComponent<ItemComponent>();
	auto cd = AddComponent<CoolDownComponent>();

	transform->SetPosition(840.f, 670.f);
	_vec3 pos = transform->GetPosition();
	transform->SetScale(0.4f, 0.55f);
	renderer->SetTexture(L"dash_icon");

	auto maskObj = MaskObj::Create(owner);
	auto maskRenderer = maskObj->GetComponent<UIRenderer>();
	maskRenderer->SetScale(0.8f, -0.55f);
	auto maskTf = maskObj->GetComponent<TransformComponent>();
	maskTf->SetPosition(pos.x, pos.y + 15.f);

	cd->Init(item, maskRenderer);
	item->SetCoolDown(true, 1.f);

	owner->AddUIObject(maskObj);

	return S_OK;
}