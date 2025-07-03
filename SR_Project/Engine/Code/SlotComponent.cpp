#include "EnginePCH.h"
#include "SlotComponent.h"
#include "UIRenderer.h"
#include "Object.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "TransformComponent.h"

void SlotComponent::SetSlotSize(SlotSize size)
{
	slotSize = size;
	ApplySlotSize();
}

void SlotComponent::ApplySlotSize()
{
	assert(base && "SlotComponent::ApplySlotSize - base is missing");
	assert(highlight && "SlotComponent::ApplySlotSize - highlight is missing");

	switch (slotSize)
	{
	case SlotSize::Small:
		base->SetScale(0.2f, 0.2f);
		highlight->SetScale(1.6f, 1.6f);
		break;

	case SlotSize::Default:
		base->SetScale(0.25f, 0.25f);
		highlight->SetScale(2.5f, 2.5f);
		break;

	case SlotSize::Large:
		base->SetScale(0.3f, 0.3f);
		highlight->SetScale(3.f, 3.f);
		break;
	}
}

void SlotComponent::OnClick()
{
	if (slotType == SlotType::Quick)
	{
		auto transform = owner->GetComponent<TransformComponent>();
		assert(transform && "SlotComponent::OnClick - TransformComponent is missing");

		originalY = transform->GetPosition().y;
		shakeTime = 0.f;
		shakeDuration = 0.05f;
		isShaking = true;
	}
}


void SlotComponent::BindRenderers(UIRenderer* _base, UIRenderer* _highlight)
{
	base = _base;
	highlight = _highlight;
}

void SlotComponent::Update(float dt)
{
	auto transform = owner->GetComponent<TransformComponent>();
	assert(transform && "SlotComponent::Update - transformComponent is missing");

	if (isShaking)
	{
		shakeTime += dt;

		if (shakeTime >= shakeDuration)
		{
			isShaking = false;
			shakeTime = 0.f;

			_vec3 pos = transform->GetPosition();
			pos.y = originalY;
			transform->SetPosition(pos);
		}
		else
		{
			float t = shakeTime / shakeDuration;
			float offset = sinf(t * D3DX_PI) * shakePower;

			_vec3 pos = transform->GetPosition();
			pos.y = originalY + offset;
			transform->SetPosition(pos);
		}
	}
}

void SlotComponent::OnHoverEnter()
{
	isHovered = true;
	highlight->SetVisible(true);
}

void SlotComponent::OnHoverExit()
{
	isHovered = false;
	highlight->SetVisible(false);
}
