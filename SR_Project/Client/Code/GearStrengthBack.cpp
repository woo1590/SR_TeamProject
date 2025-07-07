#include "pch.h"
#include "GearStrengthBack.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

GearStrengthBack* GearStrengthBack::Create(ObjectManager* owner)
{
	auto* instance = new GearStrengthBack(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT GearStrengthBack::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetPosition(470.f, 355.f);
	transform->SetScale(0.22f, 0.25f);
	renderer->SetTexture(L"gearstrength_back");
	renderer->SetRenderType(UIRenderType::Inventory);

	return S_OK;
}