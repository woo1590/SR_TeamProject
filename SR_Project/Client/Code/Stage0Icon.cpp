#include "pch.h"
#include "Stage0Icon.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

Stage0Icon* Stage0Icon::Create(ObjectManager* owner)
{
	auto instance = new Stage0Icon(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT Stage0Icon::Ready_Object()
{
	auto tf = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	renderer->SetScale(0.5f, 0.5f);
	renderer->SetTexture(L"stage0");
	renderer->SetRenderType(UIRenderType::WorldMap);

	return S_OK;
}