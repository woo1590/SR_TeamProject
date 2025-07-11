#include "pch.h"
#include "HPBarWhite.h"
#include "TransformComponent.h"
#include "ProgressBar.h"
#include "UIRenderer.h"
#include "InfoComponent.h"

HPBarWhite* HPBarWhite::Create(ObjectManager* owner)
{
	auto* instance = new HPBarWhite(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HPBarWhite::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto progress  = AddComponent<ProgressBar<EnemyInfo>>();

	transform->SetScale(0.8f, 0.8f);

	renderer->SetTexture(L"hpbar_white");
	renderer->SetPivot(UIPivot::Left); 
	
	progress->SetRenderPolicy(RenderPolicy::HideWhenFull);
	progress->SetDelay(0.6f);
	progress->SetLerpSpeed(2.5f);

	progress->SetBarDirection(BarDirection::Horizontal);

	return S_OK;
}