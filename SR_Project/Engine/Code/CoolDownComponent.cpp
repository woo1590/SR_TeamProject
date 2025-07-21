#include "EnginePCH.h"
#include "CoolDownComponent.h"
#include "Object.h"
#include "UIRenderer.h"
#include "ItemComponent.h"
#include "TransformComponent.h"

void CoolDownComponent::Init(ItemComponent* targetItem, UIRenderer* mask)
{
	itemComp = targetItem;
	maskRenderer = mask;
	maskRenderer->SetPivot(UIPivot::Top);
	maskRenderer->SetVisible(false);
}

void CoolDownComponent::Update(float dt)
{
	if (!itemComp) return;

	if (itemComp->IsCoolDown())
	{
		float fill = clamp(itemComp->GetCoolDownRatio(),0.f,1.f);
		maskRenderer->ApplyRatioVertical(fill);
		maskRenderer->SetVisible(fill > 0.f);
	}
	else
		maskRenderer->SetVisible(false);
}