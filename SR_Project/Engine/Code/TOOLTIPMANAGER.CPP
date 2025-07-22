#include "EnginePCH.h"
#include "TooltipManager.h"
#include "Object.h"
#include "UIRenderer.h"
#include "TransformCOmponent.h"
#include "FontComponent.h"

void TooltipManager::ShowTooltip(TooltipData& tip, const wstring& text, float x, float y, bool above, FontType fontType)
{
	if (!tip.obj) return;

	auto transform = tip.obj->GetComponent<TransformComponent>();
	auto font = tip.obj->GetComponent<FontComponent>();
	auto renderer = tip.obj->GetComponent<UIRenderer>();

	if (!transform || !font) return;

	float yOffset = above ? -30.f : +120.f;
	transform->SetPosition(x, y + yOffset);

	font->ClearText();

	RECT rect = {
		(LONG)x - 100,
		(LONG)(y + yOffset - 20),
		(LONG)x + 100,
		(LONG)(y + yOffset + 10)
	};

	font->AddText(text, rect, Color::White, DT_CENTER, fontType);
	font->SetVisible(true);
	if (renderer)
		renderer->SetVisible(true);

	tip.alpha = 0.f;
	tip.fadingIn = true;
}

void TooltipManager::ShowInventoryTooltip(const wstring& text, float x, float y)
{
	ShowTooltip(invTip, text, x, y, true, FontType::Large);
}

void TooltipManager::ShowWorldTooltip(const wstring& text, float x, float y)
{
	ShowTooltip(worldTip, text, x, y, false, FontType::Title);
}

void TooltipManager::ShowShopTooltip(const wstring& text, float x, float y)
{
	ShowTooltip(shopTip, text, x, y, false, FontType::Large);
}

void TooltipManager::HideTooltip(TooltipData& tip)
{
	if (!tip.obj) return;

	auto font = tip.obj->GetComponent<FontComponent>();
	auto renderer = tip.obj->GetComponent<UIRenderer>();

	if (font)
	{
		font->SetVisible(false);
		font->SetAlpha(1.f);
	}
	if (renderer)
		renderer->SetVisible(false);

	tip.alpha = 0.f;
	tip.fadingIn = false;
}

void TooltipManager::HideInventoryTooltip()
{
	HideTooltip(invTip);
}

void TooltipManager::HideWorldTooltip()
{
	HideTooltip(worldTip);
}

void TooltipManager::HideShopTooltip()
{
	HideTooltip(shopTip);
}

void TooltipManager::Update(float dt)
{
	auto fadeUpdate = [dt](TooltipData& tip)
		{
			if (!tip.fadingIn || !tip.obj) return;

			auto font = tip.obj->GetComponent<FontComponent>();
			if (!font) return;

			tip.alpha += dt * 2.f;
			if (tip.alpha >= 1.f)
			{
				tip.alpha = 1.f;
				tip.fadingIn = false;
			}
			font->SetAlpha(tip.alpha);
		};
	
	fadeUpdate(invTip);
	fadeUpdate(worldTip);
	fadeUpdate(shopTip);
}