#include "pch.h"
#include "CoolDownBar.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

CoolDownBar* CoolDownBar::Create(ObjectManager* owner)
{
	auto instance = new CoolDownBar(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT CoolDownBar::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	renderer->SetTexture(L"cooldown_front");

	return S_OK;
}