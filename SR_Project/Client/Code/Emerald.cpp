#include "pch.h"
#include "Emerald.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

Emerald* Emerald::Create(ObjectManager* owner)
{
	auto* instance = new Emerald(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT Emerald::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetPosition(1025.f, 675.f);
	
	renderer->SetScale(0.5f, 0.4f);
	renderer->SetTexture(L"icon_emerald");

	return S_OK;
}