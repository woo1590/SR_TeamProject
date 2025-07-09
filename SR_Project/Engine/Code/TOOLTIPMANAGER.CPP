#include "EnginePCH.h"
#include "TooltipManager.h"
#include "Object.h"
#include "UIRenderer.h"
#include "TransformCOmponent.h"
#include "FontComponent.h"

void TooltipManager::ShowWorldTooltip(const wstring& text, float x, float y)
{
	if (!worldTooltip) return;

	auto transform = worldTooltip->GetComponent<TransformComponent>();
	auto font      = worldTooltip->GetComponent<FontComponent>();
	auto renderer  = worldTooltip->GetComponent<UIRenderer>();

	if (!transform || !font || !renderer) return;

	transform->SetPosition(x, y + 120.f);
	font->ClearText();

	RECT rect = {(LONG)x - 150,(LONG)y + 100, (LONG)x + 150, (LONG)y + 150};

	font->AddText(text, rect, D3DXCOLOR(1, 1, 1, 1), DT_CENTER, FontType::Title);

	font->SetVisible(true);
	renderer->SetVisible(true);
}

void TooltipManager::HideWorldTooltip()
{
	if (!worldTooltip)return;
	auto font = worldTooltip->GetComponent<FontComponent>();
	auto renderer = worldTooltip->GetComponent<UIRenderer>();
	
	if (renderer)
		renderer->SetVisible(false);
	if (font)
		font->SetVisible(false);
}