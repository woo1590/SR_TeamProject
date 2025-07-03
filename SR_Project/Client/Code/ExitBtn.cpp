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

	float scale = 0.3f;

	auto base = AddComponent<UIRenderer>();
	base->SetTexture(L"exitbtn");
	base->SetScale(scale, scale);
	base->SetRenderType(UIRenderType::Inventory);

	auto highlight = AddComponent<UIRenderer>();
	highlight->SetTexture(L"exitbtn_hover");
	highlight->SetVisible(false);
	highlight->SetScale(scale, scale);
	highlight->SetRenderType(UIRenderType::Inventory);

	auto hover  = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	button->SetOnClick([]() {
		EngineCore::GetInstance()->GetRenderSystem()->SetUIRenderState(UIRenderType::MainGame);
		});
	
	button->SetBase(base);
	button->SetHighlight(highlight);

	return S_OK;
}
