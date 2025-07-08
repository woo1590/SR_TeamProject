#include "pch.h"
#include "LockNode_Back.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

LockNode_Back* LockNode_Back::Create(ObjectManager* owner)
{
	auto* instance = new LockNode_Back(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;;
}

HRESULT LockNode_Back::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetScale(0.4f, 0.4f);

	renderer->SetRenderType(UIRenderType::WorldMap);
	renderer->SetTexture(L"locked_node_back");

	return S_OK;

}