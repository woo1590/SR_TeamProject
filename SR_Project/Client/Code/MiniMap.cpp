#include "pch.h"
#include "MiniMap.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "RenderSystem.h"

MiniMap* MiniMap::Create(ObjectManager* owner)
{
	auto instance = new MiniMap(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT MiniMap::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	renderer->SetTexture(EngineCore::GetInstance()->GetRenderSystem()->GetMinimapTexture());

	transform->SetPosition(64.f, 64.f);

	return S_OK;
}