#include "EnginePCH.h"
#include "CoolDownComponent.h"
#include "Object.h"
#include "UIRenderer.h"
#include "ItemComponent.h"

CoolDownComponent* CoolDownComponent::Create(Object* owner)
{
	auto instance = new CoolDownComponent(owner);
	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT CoolDownComponent::Ready_Component()
{
	return S_OK;
}

void CoolDownComponent::Init(ItemComponent* targetItem, UIRenderer* renderer)
{
	itemComp = targetItem;
	cooldownRenderer = renderer;
}

void CoolDownComponent::Update(float dt)
{
	if (!itemComp || !cooldownRenderer) return;

	if (itemComp->IsCoolDown())
	{
		cooldownRenderer->SetVisible(true);
		float ratio = itemComp->GetCoolDownRatio();
		//cooldownRenderer->ApplyRatioVertical(ratio);
	}
	else
		cooldownRenderer->SetVisible(false); 
}