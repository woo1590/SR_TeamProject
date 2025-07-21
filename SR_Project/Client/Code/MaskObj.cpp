#include "pch.h"
#include "MaskObj.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

MaskObj* MaskObj::Create(ObjectManager* owner)
{
	auto instance = new MaskObj(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT MaskObj::Ready_Object()
{
	auto tf = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	renderer->SetPivot(UIPivot::Top);
	renderer->SetVisible(false);
	renderer->SetTexture(L"gray");
	renderer->SetAlpha(0.7f);
	renderer->SetScale(0.9f, -0.9f);

	return S_OK;
}