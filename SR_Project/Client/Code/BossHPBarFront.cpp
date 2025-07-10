#include "pch.h"
#include "BossHPBarFront.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ProgressBar.h"

BossHPBarFront* BossHPBarFront::Create(ObjectManager* owner)
{
	auto* instance = new BossHPBarFront(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT BossHPBarFront::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto hpFront = AddComponent<ProgressBar<EnemyInfo>>();

	transform->SetPosition(250.f, 100.f);
	transform->SetScale(1.5f, 0.8f);

	renderer->SetPivot(UIPivot::Left);
	renderer->SetTexture(L"enemy_hpbarfront");
	renderer->SetAlpha(0.8f);

	hpFront->SetBarDirection(BarDirection::Horizontal);
	hpFront->SetEventType(UIEventType::HP_Changed);

	return S_OK;
}