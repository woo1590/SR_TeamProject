#include "pch.h"
#include "Locked_Node.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverButtonComponent.h"
#include "HoverComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "RenderSystem.h"

Locked_Node* Locked_Node::Create(ObjectManager* owner)
{
	auto* instance = new Locked_Node(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;;
}

HRESULT Locked_Node::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.3f, 0.3f);

	auto base  = AddComponent<UIRenderer>();
	base->SetRenderType(UIRenderType::WorldMap);
	base->SetTexture(L"locked_node");

	auto highlight = AddComponent<UIRenderer>();
	highlight->SetRenderType(UIRenderType::WorldMap);
	highlight->SetTexture(L"locked_node_hover");
	highlight->SetScale(0.4f, 0.4f);
	highlight->SetVisible(false);

	auto hover  = AddComponent<HoverComponent>();
	auto button = AddComponent<HoverButtonComponent>();

	button->BindRenderers(base, highlight);
	button->SetOnClick([]() {

		});
	button->SetHighlightScale({0.4f, 0.4f}, {0.6f, 0.6f});
	
	hover->SetCallBack([button](bool over) {  
		button->SetHoverState(over);
		}); 
	return S_OK;

}