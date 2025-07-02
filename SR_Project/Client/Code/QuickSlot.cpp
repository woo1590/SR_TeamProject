#include "pch.h"
#include "QuickSlot.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "SlotComponent.h"

// System
#include "InputSystem.h"
#include "EngineCore.h"

QuickSlot* QuickSlot::Create(ObjectManager* owner)
{
	auto* instance = new QuickSlot(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT QuickSlot::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	
	auto base = AddComponent<UIRenderer>();
	base->SetTexture(baseTex);
	base->SetScale(0.25f, 0.25f);

	auto highlight = AddComponent<UIRenderer>();
	highlight->SetTexture(highlightTex);
	highlight->SetScale(2.f, 2.f);
	highlight->SetVisible(false);

	auto slot = AddComponent<SlotComponent>();
	slot->SetSlotType(SlotType::Quick);
	slot->BindRenderers(base, highlight);

	return S_OK;
}

void QuickSlot::Update(float dt)
{
	Object::Update(dt);

	auto transform = GetComponent<TransformComponent>();
	auto renderer = GetComponent<UIRenderer>();
	auto slot = GetComponent<SlotComponent>();

	const auto& input = EngineCore::GetInstance()->GetInputSystem();

	_vec3 mousePos = input->GetMousePos();

	const _vec3 pos = transform->GetPosition();
	const _vec2 scale = renderer->GetScale();

	LONG width = LONG(scale.x * renderer->GetFullWidth());
	LONG height = LONG(scale.y * renderer->GetFullHeight());
	LONG left = LONG(pos.x - width * 0.5f);
	LONG top = LONG(pos.y - height * 0.5f);

	RECT rect = {left, top, left + width, top + height};

	bool mouseOver = PtInRect(&rect, POINT{(LONG)mousePos.x, (LONG)mousePos.y});

	if (mouseOver != isHovered)
	{
		isHovered = mouseOver;
		slot->OnHover(isHovered);
	}
	
	if (mouseOver && input->IsKeyPressed(KEY::LBUTTON))
		slot->OnClick();
}