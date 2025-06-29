#include "pch.h"
#include "HPBarBack.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

HPBarBack* HPBarBack::Create(ObjectManager* owner, ObjectType type)
{
	auto* instance = new HPBarBack(owner, type);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HPBarBack::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetPosition(600.f, 600.f);

	renderer->SetTexture(L"hpbar_back");

	return S_OK;
}