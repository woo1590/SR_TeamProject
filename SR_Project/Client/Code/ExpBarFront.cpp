#include "pch.h"
#include "ExpBarFront.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ProgressBar.h"

ExpBarFront* ExpBarFront::Create(ObjectManager* owner)
{
	auto* instance = new ExpBarFront(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ExpBarFront::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto exp = AddComponent<ProgressBar<PlayerInfo>>();
	
	exp->SetBarDirection(BarDirection::Horizontal);

	transform->SetPosition(700.f, 717.f);
	
	renderer->SetPivot(UIPivot::Left);
	renderer->SetTexture(L"expbar_front");

	return S_OK;
}