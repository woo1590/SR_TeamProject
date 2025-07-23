#include "pch.h"
#include "MapNode_Front.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverButtonComponent.h"
#include "HoverComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "Scene.h"
#include "UIManager.h"
#include "SceneManager.h"
#include "TooltipManager.h"

MapNode_Front* MapNode_Front::Create(ObjectManager* owner)
{
	auto* instance = new MapNode_Front(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;;
}

HRESULT MapNode_Front::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.4f, 0.5f);

	auto base  = AddComponent<UIRenderer>();
	base->SetRenderType(UIRenderType::WorldMap);
	base->SetTexture(L"map_node_front");

	auto highlight = AddComponent<UIRenderer>();
	highlight->SetRenderType(UIRenderType::WorldMap);
	highlight->SetTexture(L"map_node_hover");
	highlight->SetScale(0.5f, 0.3f);
	highlight->SetVisible(false);

	auto hover     = AddComponent<HoverComponent>();
	auto button    = AddComponent<HoverButtonComponent>();
	button->SetHighlightScale({0.5f, 0.3f}, {0.75f,0.45f});
	button->BindRenderers(base, highlight);

	hover->SetCallBack([this,button](bool over) {
		button->SetHoverState(over);
		auto tooltip = GetScene()->GetUIManager()->GetTooltip();
		auto pos = GetComponent<TransformComponent>()->GetWorldPosition();

		if (over)
		{
			const wchar_t* tip = nullptr;
			switch (stage)
			{
			case StageSelect::Stage1: tip = L"ÇÏ´Ã¼¶"; break;
			case StageSelect::Stage2: tip = L"¿£´õ ¿ùµå"; break;
			default:                  tip = L"¸¶À»"; break;
			}
			tooltip->ShowWorldTooltip(tip, pos.x, pos.y - 40.f);
		}
		else
			tooltip->HideWorldTooltip();
		});
	return S_OK;
}