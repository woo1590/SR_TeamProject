#include "pch.h"
#include "ExitBtn.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ButtonComponent.h"
#include "HoverComponent.h"
#include "EngineCore.h"
#include "RenderSystem.h"

ExitBtn* ExitBtn::Create(ObjectManager* owner)
{
	auto* instance = new ExitBtn(owner);

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

	button->SetOnClick([] {
		EngineCore::GetInstance()->GetRenderSystem()->SetUIRenderState(UIRenderType::MainGame);
		});

	return S_OK;
}
