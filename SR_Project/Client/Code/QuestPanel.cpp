#include "pch.h"
#include "QuestPanel.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"

QuestPanel* QuestPanel::Create(ObjectManager* owner)
{
	auto* instance = new QuestPanel(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT QuestPanel::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto font = AddComponent<FontComponent>();

	transform->SetPosition(1100.f, 300.f);
	transform->SetScale(0.3f, 0.2f);

	renderer->SetVisible(false);
	renderer->SetTexture(L"questpanel");

	return S_OK;
}