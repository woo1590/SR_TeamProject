#include "EnginePCH.h"
#include "SlotComponent.h"
#include "UIRenderer.h"
#include "Object.h"

void SlotComponent::SetSlotSize(SlotSize size)
{
	slotSize = size;
	ApplySlotSize();
}

void SlotComponent::ApplySlotSize()
{
	if (!base || !highlight) return;

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

}

void SlotComponent::OnHover(bool over)
{
	if (isHovered == over)
		return;

	isHovered = over;

	if (highlight)
		highlight->SetVisible(isHovered);
}

void SlotComponent::BindRenderers(UIRenderer* _base, UIRenderer* _highlight)
{
	base = _base;
	highlight = _highlight;
}