#include "pch.h"
#include "DashUI.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

DashUI* DashUI::Create(ObjectManager* owner)
{
	auto* instance = new DashUI(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT DashUI::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetPosition(840.f, 670.f);
	
	transform->SetScale(0.4f, 0.55f);
	renderer->SetTexture(L"dash_icon");

	return S_OK;
}