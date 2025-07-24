#include "pch.h"
#include "Stage2Icon.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

Stage2Icon* Stage2Icon::Create(ObjectManager* owner)
{
	auto instance = new Stage2Icon(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT Stage2Icon::Ready_Object()
{
	auto tf = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	tf->SetScale(0.12f, 0.12f);

	renderer->SetTexture(L"enderworld");
	renderer->SetRenderType(UIRenderType::WorldMap);

	return S_OK;
}