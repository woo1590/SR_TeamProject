#include "pch.h"
#include "WingRight.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "IntroAnimComponent.h"

WingRight* WingRight::Create(ObjectManager* owner)
{
	auto instance = new WingRight(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT WingRight::Ready_Object()
{
	auto tf = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	tf->SetScale(0.5f, 0.5f);
	tf->SetPosition(WINCX * 0.5f + 150, 30.f);
	renderer->SetTexture(L"wingright");
	renderer->SetPivot(UIPivot::Top);
	renderer->SetAlpha(0.8f);

	auto anim = AddComponent<IntroAnimComponent>();

	return S_OK;
}