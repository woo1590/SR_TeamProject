#include "pch.h"
#include "MouseLeftUI.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

MouseLeftUI* MouseLeftUI::Create(ObjectManager* owner)
{
	auto* instance = new MouseLeftUI(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT MouseLeftUI::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetPosition(500.f, 500.f);
	
	renderer->SetTexture(L"mouse_left");

	return S_OK;
}