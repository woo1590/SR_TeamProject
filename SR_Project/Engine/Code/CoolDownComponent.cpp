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
	
	maskRenderer->SetLayer(owner->GetComponent<UIRenderer>()->GetLayer() + 1);
	maskRenderer->SetAlpha(0.7f);
}

void CoolDownComponent::Update(float dt)
{
	if (!itemComp) return;
	bool cooling = itemComp->IsCoolDown();

    if (cooling)
    {
        wasCooling = true;
        float fill = clamp(itemComp->GetCoolDownRatio(), 0.f, 1.f);
        maskRenderer->ApplyRatioVertical(fill);
        maskRenderer->SetVisible(fill > 0.f);
    }
    else
    {
        if (wasCooling && onCompleteCb)
            onCompleteCb();
        wasCooling = false;

        maskRenderer->SetVisible(false);
    }
}