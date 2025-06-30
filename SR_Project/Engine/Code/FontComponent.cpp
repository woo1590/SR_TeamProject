#include "EnginePCH.h"
#include "FontComponent.h"
#include "GraphicDevice.h"

FontComponent::FontComponent(Object* owner)
	:ObjectComponent(owner)
{
	CreateFontResource();
	color = ToD3DXColor(Color::White);
}

FontComponent::~FontComponent()
{
	if (font)
		font->Release();
}

FontComponent* FontComponent::Create(Object* owner)
{
	return new FontComponent(owner);
}

HRESULT FontComponent::CreateFontResource()
{
	return D3DXCreateFont(
		GraphicDevice::GetInstance()->GetDevice(),
		18, 0, FW_NORMAL, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"¸¼Àº °íµñ",
		&font);
}

void FontComponent::SetFontType(FontType type)
{
	if (font)
		font->Release();

	int height = 18;
	int weight = FW_NORMAL;

	switch (type)
	{
	case FontType::Regular: height = 18; weight = FW_NORMAL; break;
	case FontType::Bold:    height = 18; weight = FW_BOLD;   break;
	case FontType::Small:   height = 12; weight = FW_NORMAL; break;
	case FontType::Large:   height = 28; weight = FW_NORMAL; break;
	case FontType::Title:   height = 36; weight = FW_BOLD;   break;
	case FontType::Custom:  height = 18; weight = FW_NORMAL; break;
	}

	D3DXCreateFont(GraphicDevice::GetInstance()->GetDevice(), height, 0, weight, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH || FF_DONTCARE, "¸¼Àº °íµñ", &font);
}

void FontComponent::Render()
{
	if (!font || text.empty()) return;

	font->DrawTextW(nullptr, text.c_str(), -1, &rect, format, color);
}