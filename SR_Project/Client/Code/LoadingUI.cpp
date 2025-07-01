#include "pch.h"
#include "LoadingUI.h"
#include "TransformComponent.h"
#include "UIRenderer.h"


LoadingUI* LoadingUI::Create(ObjectManager* owner)
{
	auto* instance = new LoadingUI(owner);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT LoadingUI::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	
	transform->SetPosition(500.f, 500.f);
	renderer->SetTexture(L"loadingscene");

	return S_OK;
}
