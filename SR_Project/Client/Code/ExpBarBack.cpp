#include "pch.h"
#include "ExpBarBack.h"
#include "UIRenderer.h"
#include "TransformComponent.h"

ExpBarBack* ExpBarBack::Create(ObjectManager* owner)
{
	auto* instance = new ExpBarBack(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ExpBarBack::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetPosition(700.f,717.f);
	renderer->SetTexture(L"expbar_back");

	return S_OK;
}