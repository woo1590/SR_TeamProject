#include "pch.h"
#include "ExitBtn.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ButtonComponent.h"
#include "HoverComponent.h"

ExitBtn* ExitBtn::Create(ObjectManager* owner)
{
	auto* instance = new ExitBtn(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ExitBtn::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(1200.f, 100.f);

	float scale = 0.3f;

	auto base = AddComponent<UIRenderer>();
	base->SetTexture(L"exitbtn");
	base->SetScale(scale, scale);

	auto highlight = AddComponent<UIRenderer>();
	highlight->SetTexture(L"exitbtn_hover");
	highlight->SetVisible(false);
	highlight->SetScale(scale, scale);

	auto hover  = AddComponent<HoverComponent>();
	auto button = AddComponent<ButtonComponent>();

	button->SetRenderer(highlight);
	
	button->SetOnClick([this]() {
		targetPanel->SetVisible(false);
		});

	button->Ready_Component();

	return S_OK;
}