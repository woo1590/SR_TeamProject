#include "pch.h"
#include "WorldMapTextPanel.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"
#include "HoverComponent.h"
#include "ButtonComponent.h"

WorldMapTextPanel* WorldMapTextPanel::selectedPanel = nullptr;

WorldMapTextPanel* WorldMapTextPanel::Create(ObjectManager* owner)
{
	auto* instance = new WorldMapTextPanel(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT WorldMapTextPanel::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto font      = AddComponent<FontComponent>();
	auto hover     = AddComponent<HoverComponent>();
	auto button    = AddComponent<ButtonComponent>();

	transform->SetScale(0.8f, 1.f);
	renderer->SetTexture(L"worldmap_textpanel");
	renderer->SetRenderType(UIRenderType::WorldMap);

	hover->SetCallBack([transform](bool over) {
		if (over)
			transform->SetScale(0.9f, 1.1f);
		else
			transform->SetScale(0.8f, 1.f);
		});

	button->SetOnClick([this]() {
		if (selectedPanel && selectedPanel != this)
			selectedPanel->isSelected = false;
		isSelected = true;
		selectedPanel = this;
		});

	return S_OK;
}

void WorldMapTextPanel::Update(float dt)
{
	Object::Update(dt);

	auto font = GetComponent<FontComponent>();
	auto transform = GetComponent<TransformComponent>();
	font->ClearText();

	_vec3 pos = transform->GetPosition();

	RECT textRect = {
		(LONG)pos.x - 100, (LONG)pos.y - 10,
		(LONG)pos.x + 100, (LONG)pos.y + 70
	};
	Color color = isSelected ? Color::White : Color::Gray;

	font->AddText(displayText, textRect, color, DT_CENTER, FontType::Title);
}
