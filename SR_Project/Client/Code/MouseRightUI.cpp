#include "pch.h"
#include "MouseRightUI.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

MouseRightUI* MouseRightUI::Create(ObjectManager* owner)
{
	auto* instance = new MouseRightUI(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT MouseRightUI::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetPosition(965.f, 680.f);

	renderer->SetScale(0.5f, 0.5f);
	renderer->SetTexture(L"mouse_right");

	return S_OK;
}