#include "pch.h"
#include "HPBarFront.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ProgressBar.h"

HPBarFront* HPBarFront::Create(ObjectManager* owner)
{
	auto* instance = new HPBarFront(owner);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HPBarFront::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto hpUI      = AddComponent<ProgressBar<PlayerInfo>>();

	renderer->SetTexture(L"hpbar_front");
	renderer->SetPivot(UIPivot::Bottom);

	renderer->SetScale(0.2f, 0.4f);

	transform->SetPosition(615.f, 700.f);

	return S_OK;
}