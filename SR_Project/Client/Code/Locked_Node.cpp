#include "pch.h"
#include "Locked_Node.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverButtonComponent.h"
#include "HoverComponent.h"
#include "UIManager.h"
#include "TooltipManager.h"
#include "Scene.h"
#include "ObjectManager.h"

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
	button->SetHighlightScale({0.4f, 0.4f}, {0.6f, 0.6f});

	button->SetOnClick([]() {

		});	
	
	hover->SetCallBack([this,button](bool over) {  
		button->SetHoverState(over);

		auto* scene = this->GetOwner()->GetOwner();
		if (!scene) return;

		auto* uiMgr = scene->GetUIManager();
		if (!uiMgr) return;

		auto* tooltip = uiMgr->GetTooltip();
		if (!tooltip) return;

		auto pos = button->GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();

		if (over)
			tooltip->ShowWorldTooltip(L"Àá±Ý", pos.x, pos.y - 40.f);
		else
			tooltip->HideWorldTooltip();
		}); 
	return S_OK;

}