#include "pch.h"
#include "EnemyHPBarFront.h"
#include "UIRenderer.h"
#include "ProgressBar.h"
#include "GraphicDevice.h"
#include "RenderSystem.h"
#include "EngineCore.h"
#include "FontComponent.h"
#include "WorldUIComponent.h"

EnemyHPBarFront* EnemyHPBarFront::Create(ObjectManager* owner)
{
	auto* instance = new EnemyHPBarFront(owner);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT EnemyHPBarFront::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto hpBar     = AddComponent<ProgressBar<EnemyInfo>>();
	auto font      = AddComponent<FontComponent>();
	auto worldUI = AddComponent<WorldUIComponent>();

	worldUI->SetBaseScale(0.055f, 0.2f);
	worldUI->SetScaleRange(0.5f, 1.5f);
	worldUI->SetOffset(_vec3(-4.f, 2.f, 0.f));

	hpBar->SetBarDirection(BarDirection::Horizontal);
	hpBar->SetEventType(UIEventType::HP_Changed);
	
	renderer->SetAlpha(0.8f);
	renderer->SetPivot(UIPivot::Left);
	renderer->SetRenderType(UIRenderType::MainGame);
	renderer->SetTexture(L"boss_hpbarfront");

	hpBar->SetRenderPolicy(RenderPolicy::HideWhenFull);

	return S_OK;
}

void EnemyHPBarFront::SetTarget(TransformComponent* target)
{
	GetComponent<WorldUIComponent>()->SetTarget(target);
}