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

void TooltipManager::ShowInventoryTooltip(const wstring& text, float x, float y)
{
	if (!inventoryTooltip) return;

	auto transform = inventoryTooltip->GetComponent<TransformComponent>();
	auto font = inventoryTooltip->GetComponent<FontComponent>();
	if (!transform || !font) return;

	transform->SetPosition(x, y - 30.f);

	font->ClearText();

	RECT rect = {(LONG)x - 100, 
		         (LONG)y - 60, 
		         (LONG)x + 100, 
		         (LONG)y - 30};
	font->AddText(text, rect, Color::White, DT_CENTER, FontType::Large);
	font->SetVisible(true);

	invAlpha = 0.f;
	invFadingIn = true;
}

void TooltipManager::HideInventoryTooltip()
{
	if (!inventoryTooltip) return;
	
	if (auto font = inventoryTooltip->GetComponent<FontComponent>())
	{
		font->SetVisible(false);
		font->SetAlpha(1.f);
	}
	invAlpha = 0.f;
	invFadingIn = false;
}

void TooltipManager::Update(float dt)
{
	if (invFadingIn && inventoryTooltip)
	{
		auto font = inventoryTooltip->GetComponent<FontComponent>();
		if (!font) return;

		invAlpha += dt * 2.f;
		if (invAlpha >= 1.f)
		{
			invAlpha = 1.f;
			invFadingIn = false;
		}
		font->SetAlpha(invAlpha);
	}
}