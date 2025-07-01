#include "pch.h"
#include "HotBarBack.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

HotBarBack* HotBarBack::Create(ObjectManager* owner)
{
	auto* instance = new HotBarBack(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HotBarBack::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();

	transform->SetPosition(WINCX * 0.5f, WINCY * 0.96f);
	
	renderer->SetScale(0.4f, 0.3f);
	renderer->SetTexture(L"hotbar_back");

	return S_OK;
}