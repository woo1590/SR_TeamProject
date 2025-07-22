#include "pch.h"
#include "Stage1Icon.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

Stage1Icon* Stage1Icon::Create(ObjectManager* owner)
{
	auto instance = new Stage1Icon(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT Stage1Icon::Ready_Object()
{
	auto tf = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	tf->SetScale(0.5f, 0.5f);

	renderer->SetTexture(L"stage1");
	renderer->SetRenderType(UIRenderType::WorldMap);

	return S_OK;
}