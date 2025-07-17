#include "pch.h"
#include "TooltipObj.h"
#include "TransformComponent.h"
#include "InfoComponent.h"
#include "TooltipBuilder.h"

TooltipObj* TooltipObj::Create(ObjectManager* owner)
{
	auto* instance = new TooltipObj(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT TooltipObj::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(1100.f, 200.f);
	transform->SetScale(2.f, 2.f);
	renderer = AddComponent<UIRenderer>();
	renderer->SetRenderType(UIRenderType::Inventory);
	renderer->SetVisible(false);

	font = AddComponent<FontComponent>();

	return S_OK;
}

void TooltipObj::Update(float dt)
{
	if (!inventory) return;

	auto* selectedSlot = inventory->GetSelectedSlot();
	if (!selectedSlot || !selectedSlot->HasItem())
	{
		if (prevSlot != nullptr)
		{
			font->ClearText();
			renderer->SetVisible(false);
			prevSlot = nullptr;
		}
		return;
	}

	if (selectedSlot == prevSlot) return;

	auto* itemObj = selectedSlot->GetItem();
	if (!itemObj) return;

	auto* infoComp = itemObj->GetComponent<InfoComponent<ItemInfo>>();
	if (!infoComp) return;

	const auto& info = infoComp->GetInfo();

	font->ClearText();

	vector<TooltipLine> lineToRender = TooltipBuilder::BuildTooltip(info);

	constexpr float startX = 1010.f;
	constexpr float startY = 290.f;
	constexpr float maxWidth = 270.f;
	constexpr float lineSpacing = 10.f;
	float curY = startY;

	for (const auto& line : lineToRender)
	{
		RECT measuredRect = font->MeasureText(line.fontType, line.text);
		long lineHeight = measuredRect.bottom - measuredRect.top;

		RECT renderRect;
		renderRect.left = static_cast<LONG>(startX);
		renderRect.top = static_cast<LONG>(curY);
		renderRect.right = static_cast<LONG>(startX + maxWidth);
		renderRect.bottom = static_cast<LONG>(curY + lineHeight);

		font->AddText(line.text, renderRect, line.color, DT_LEFT | DT_TOP | DT_WORDBREAK, line.fontType);

		curY += lineHeight + lineSpacing;
	}

	renderer->SetTexture(info.renderKey); 
	renderer->SetScale(0.5f, 0.5f);
	renderer->SetVisible(true);

	prevSlot = selectedSlot;
}
