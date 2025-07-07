#include "pch.h"
#include "HPBarBack.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"

HPBarBack* HPBarBack::Create(ObjectManager* owner)
{
	auto* instance = new HPBarBack(owner);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HPBarBack::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();

	transform->SetScale(0.3f, 0.25f);

	transform->SetPosition(WINCX * 0.5f, WINCY * 0.9f);

	renderer->SetTexture(L"hpbar_back");
	return S_OK;
}
