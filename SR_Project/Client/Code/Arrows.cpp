#include "pch.h"
#include "Arrows.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"

Arrows* Arrows::Create(ObjectManager* owner)
{
	auto* instance = new Arrows(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT Arrows::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto font = AddComponent<FontComponent>();

	transform->SetScale(0.25f, 0.2f);
	transform->SetPosition(940.f, 655.f);
	renderer->SetTexture(L"arrows");

	return S_OK;
}

void Arrows::Update(float dt)
{
	Object::Update(dt);

	auto font = GetComponent<FontComponent>();
	font->ClearText();

	font->AddText(L"\u221E", {880, 620, 940, 670}, Color::White, DT_CENTER, FontType::QuestTitle);
}
