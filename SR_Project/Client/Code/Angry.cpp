#include "pch.h"
#include "Angry.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

Angry* Angry::Create(ObjectManager* owner)
{
	auto* instance = new Angry(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance; 
}

HRESULT Angry::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetScale(0.2f, 0.15f);
	transform->SetPosition(315.f, 300.f);

	renderer->SetRenderType(UIRenderType::QuestUI);
	renderer->SetTexture(L"angry");

	return S_OK;
}