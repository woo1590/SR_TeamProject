#include "pch.h"
#include "DialogRect.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

DialogRect* DialogRect::Create(ObjectManager* owner)
{
	auto instance = new DialogRect(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT DialogRect::Ready_Object()
{
	auto tf = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	renderer->SetTexture(L"gray2");
	renderer->SetRenderType(UIRenderType::QuestUI);
	renderer->SetLayer(15);

	return S_OK;
}