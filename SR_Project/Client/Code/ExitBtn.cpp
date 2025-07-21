#include "pch.h"
#include "ExitBtn.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ButtonComponent.h"
#include "HoverComponent.h"
#include "EngineCore.h"
#include "RenderSystem.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "Scene.h"
#include "Player.h"

ExitBtn* ExitBtn::Create(ObjectManager* owner, ExitBtnType type)
{
	auto* instance = new ExitBtn(owner);

	instance->btnType = type;

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;

}

HRESULT ExitBtn::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(1230.f, 90.f);
	transform->SetScale(0.3f,0.3f);

	auto base = AddComponent<UIRenderer>();
	auto hover = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	base->SetRenderType(UIRenderType::Inventory);
	base->SetTexture(L"exitbtn");
	
	button->SetRenderer(base);
	button->SetTextures(L"exitbtn", L"exitbtn_hover");

	button->SetOnClick([this] {
		auto player = static_cast<Player*>(owner->GetFrontObject(ObjectType::Player));
		if (btnType == ExitBtnType::Inventory)
			player->SetInventoryMode(false);
		else if (btnType == ExitBtnType::WorldMap)
		{
			EngineCore::GetInstance()->GetRenderSystem()->SetUIRenderState(UIRenderType::MainGame);
		}
		});

	return S_OK;
}
