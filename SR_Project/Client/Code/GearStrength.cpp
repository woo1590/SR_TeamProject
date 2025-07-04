#include "pch.h"
#include "GearStrength.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"

GearStrength* GearStrength::Create(ObjectManager* owner)
{
	auto* instance = new GearStrength(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT GearStrength::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto font = AddComponent<FontComponent>();

	transform->SetPosition(470.f, 355.f);
	renderer->SetTexture(L"gearstrength");
	renderer->SetRenderType(UIRenderType::Inventory);
	renderer->SetScale(0.36f, 0.42f);

	return S_OK;
}
