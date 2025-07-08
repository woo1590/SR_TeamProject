#include "pch.h"
#include "MapNode.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ButtonComponent.h"
#include "HoverComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "RenderSystem.h"

MapNode* MapNode::Create(ObjectManager* owner)
{
	auto* instance = new MapNode(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;;
}

HRESULT MapNode::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto hover     = AddComponent<HoverComponent>();
	auto button    = AddComponent<ButtonComponent>();

	transform->SetScale(0.3f, 0.3f);

	renderer->SetRenderType(UIRenderType::WorldMap);
	renderer->SetTexture(L"map_node");

	return S_OK;

}