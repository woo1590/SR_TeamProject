#include "EnginePCH.h"
#include "HpComponent.h"
#include "PlayerInfoComponent.h"
#include "UIRenderer.h"

HpComponent::HpComponent(Object* owner) :ObjectComponent(owner){}

HpComponent* HpComponent::Create(Object* owner)
{
	HpComponent* instance = new HpComponent(owner);
	return (FAILED(instance->Ready_Component(owner))) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HpComponent::Ready_Component(Object* owner)
{
	front = owner->AddComponent<UIRenderer>(); 
	front->SetTexture(L"hpbar_front");

	return front ? S_OK : E_FAIL;
}

void HpComponent::AttachPlayerInfo(PlayerInfoComponent* info)
{
	if (!info) return;

	info->Attach(this);
	maxHp = curHp = info->GetMaxHp();
}

void HpComponent::OnNotify(const NotifyEvent& event)
{
	if (event.type != static_cast<int>(NotifyType::HP_Changed)) return;

	auto* hp = static_cast<HPData*>(event.data);
	curHp = hp->curHp;
	maxHp = hp->maxHp;
	targetRatio = static_cast<float>(curHp) / maxHp;
}

void HpComponent::Update(float dt)
{
	if (abs(curRatio - targetRatio) > 0.01f)
	{
		float t = clamp(dt * speed, 0.f, 1.f);
		curRatio = lerp(curRatio, targetRatio, t);		
		ApplyRatio(front, curRatio);
	}
}

void HpComponent::ApplyRatio(UIRenderer* renderer, float ratio)
{
	if (renderer)
		renderer->ApplyRatio(ratio);
}
