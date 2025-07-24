#include "EnginePCH.h"
#include "HoverButtonComponent.h"
#include "Object.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "EngineCore.h"
#include "SoundManager.h"

HoverButtonComponent* HoverButtonComponent::Create(Object* owner)
{
	auto* instance = new HoverButtonComponent(owner);
	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HoverButtonComponent::Ready_Component()
{
	return S_OK;
}

void HoverButtonComponent::BindRenderers(UIRenderer* _base, UIRenderer* _hover)
{
	base = _base;
	hover = _hover;
	if (hover)
		hover->SetVisible(false);
}

void HoverButtonComponent::SetHighlightScale(_vec2 _base, _vec2 _max)
{
	baseScale = _base;
	maxScale = _max;
	curScale = _max.x;
}

void HoverButtonComponent::SetHoverState(bool on)
{
	isHovered = on;

	if (!hover) return;

	if (on)
	{
		hover->SetVisible(true);
		curScale = maxScale.x;
	}
	else
		hover->SetVisible(false);
}

void HoverButtonComponent::Update(float dt)
{
	if (!hover || !hover->IsVisible())
		return;

	if (curScale > 0.f)
		curScale += (0.f - curScale) * dt * scaleSpeed;

	_vec2 scale;
	scale.x = maxScale.x * curScale + baseScale.x * (1.f - curScale);
	scale.y = maxScale.y * curScale + baseScale.y * (1.f - curScale);
	hover->SetScale(scale.x, scale.y);

	if (isHovered)
	{
		const auto& input = EngineCore::GetInstance()->GetInputSystem();
		if (input->IsKeyPressed(KEY::LBUTTON) && onClick)
		{
			EngineCore::GetInstance()->GetSoundManager()->PlaySFX("MouseClick1");
			onClick();
		}
	}
}