#include "pch.h"
#include "ExpBarFront.h"
#include "TransformComponent.h"
#include "ExpComponent.h"
#include "UIRenderer.h"

ExpBarFront* ExpBarFront::Create(ObjectManager* owner)
{
	auto* instance = new ExpBarFront(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ExpBarFront::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto expUI     = AddComponent<ExpComponent>();

	transform->SetPosition(500.f, 500.f);
	
	renderer->SetPivot(UIPivot::Left);
	renderer->SetScale(10.f, 1.f);
	renderer->SetTexture(L"expbar_front");

	return S_OK;
}