#include "EnginePCH.h"
#include "HoverComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "UIRenderer.h"
#include "Object.h"
#include "TransformComponent.h"
#include "SlotComponent.h"
#include "SoundManager.h"
#include "EngineCore.h"

void HoverComponent::Update(float dt)
{
	const auto& input = EngineCore::GetInstance()->GetInputSystem();
	const _vec3 mousePos = input->GetMousePos();

	auto transform = owner->GetComponent<TransformComponent>();
	auto renderer = owner->GetComponent<UIRenderer>();
	
	assert(transform && "HoverComponent::Update - transformComponent is Missing");
	assert(renderer && "HoverComponent::Update - UIRenderer is Missing");

	_vec3 pos = transform->GetWorldPosition();

	_vec3 scale = transform->GetScale();
	_vec2 overrideScale = renderer->GetScale();

	if (overrideScale.x != 1.f || overrideScale.y != 1.f)
	{
		scale.x = overrideScale.x;
		scale.y = overrideScale.y;
	}
	LONG width = LONG(scale.x * renderer->GetFullWidth());
	LONG height = LONG(scale.y * renderer->GetFullHeight());

	RECT rect = {
		LONG(pos.x - width  * 0.5f),
		LONG(pos.y - height * 0.5f),
		LONG(pos.x + width  * 0.5f),
		LONG(pos.y + height * 0.5f)
	};

	bool mouseOver = PtInRect(&rect, POINT{(LONG)mousePos.x, (LONG)mousePos.y});

	if (mouseOver != isHovered)
	{
		isHovered = mouseOver;

		if (callBack)
			callBack(isHovered);

		if (auto slot = owner->GetComponent<SlotComponent>())
			isHovered ? slot->OnHoverEnter() : slot->OnHoverExit();
	}

	if (mouseOver && input->IsKeyPressed(KEY::LBUTTON))
	{
		if (auto slot = owner->GetComponent<SlotComponent>())
		{
			EngineCore::GetInstance()->GetSoundManager()->PlaySFX("MouseClick1");
			slot->OnClick();
		}
	}

	if (mouseOver && input->IsKeyPressed(KEY::RBUTTON))
	{
		if (onRightClick)
		{
			EngineCore::GetInstance()->GetSoundManager()->PlaySFX("MouseClick2");
			onRightClick();
		}
	}

	if (updateCallBack)
		updateCallBack(isHovered);
}