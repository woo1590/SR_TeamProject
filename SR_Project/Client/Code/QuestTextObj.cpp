#include "pch.h"
#include "QuestTextObj.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"

QuestTextObj* QuestTextObj::Create(ObjectManager* owner)
{
	auto* instance = new QuestTextObj(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT QuestTextObj::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto font = AddComponent<FontComponent>();

	renderer->SetTexture(L"debugui");
	renderer->SetRenderType(UIRenderType::Always);

	return S_OK;
}