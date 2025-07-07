#include "pch.h"
#include "ScrollBack.h"
#include "UIRenderer.h"
#include "TransformComponent.h"

ScrollBack* ScrollBack::Create(ObjectManager* owner)
{
	auto* instance = new ScrollBack(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ScrollBack::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	transform->SetScale(1.f, 0.26f);
	transform->SetPosition(950.f, 425.f);
	renderer->SetTexture(L"scroll_back");
	renderer->SetRenderType(UIRenderType::Inventory);

	return S_OK;
}
