#include "pch.h"
#include "WorldTooltip.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"

WorldTooltip* WorldTooltip::Create(ObjectManager* owner)
{
	auto* instance = new WorldTooltip(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT WorldTooltip::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.05f, 0.05f);
	
	renderer = AddComponent<UIRenderer>();
	renderer->SetRenderType(UIRenderType::WorldMap);
	renderer->SetTexture(L"questpanel");
	renderer->SetVisible(false);

	font = AddComponent<FontComponent>();

	return S_OK;
}

