#include "EnginePCH.h"
#include "FontComponent.h"
#include "GraphicDevice.h"
#include "TransformComponent.h"
#include "Object.h"

FontComponent::FontComponent(Object* owner)
	:ObjectComponent(owner)
{
	AddFontResourceEx("../../Client/Resource/Asset/Jehyun/Font/NotoSansJP-Medium.ttf", FR_PRIVATE,0);
	AddFontResourceEx("../../Client/Resource/Asset/Jehyun/Font/NotoSansKR-Medium.ttf", FR_PRIVATE, 0);
	AddFontResourceEx("../../Client/Resource/Asset/Jehyun/Font/NotoSansMono-Bold.ttf", FR_PRIVATE, 0);
	AddFontResourceEx("../../Client/Resource/Asset/Jehyun/Font/NotoSansSC-Medium.ttf", FR_PRIVATE, 0);
	AddFontResourceEx("../../Client/Resource/Asset/Jehyun/Font/NotoSansTC-Medium.ttf", FR_PRIVATE, 0);
	AddFontResourceEx("../../Client/Resource/Asset/Jehyun/Font/PerfectDOSVGA437.ttf",  FR_PRIVATE, 0);
	AddFontResourceEx("../../Client/Resource/Asset/Jehyun/Font/Roboto-Bold.ttf",       FR_PRIVATE, 0);
	AddFontResourceEx("../../Client/Resource/Asset/Jehyun/Font/Roboto-Regular.ttf",    FR_PRIVATE, 0);

	CreateFontResource();
}

FontComponent::~FontComponent()
{
	for (auto& [type, font] : fontMap)
	{
		if (font)
			font->Release();
	}
}

FontComponent* FontComponent::Create(Object* owner)
{
	return new FontComponent(owner);
}
HRESULT FontComponent::CreateFontResource()
{
	auto device = GraphicDevice::GetInstance()->GetDevice();

	auto create = [&](FontType type, int height, int weight, const wchar_t* fontName) {
		ID3DXFont* font = nullptr;

		if (SUCCEEDED(D3DXCreateFontW(device, height, 0, weight, 1, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			fontName, &font)))
			fontMap[type] = font;
		else
			assert(false && "Font creation failed");
		};

	create(FontType::Regular,    24, FW_NORMAL, L"Noto Sans JP Medium");
	create(FontType::QuestTitle, 36, FW_BOLD,   L"Noto Sans JP Medium");
	create(FontType::Small,      12, FW_NORMAL, L"Noto Sans KR Medium");
	create(FontType::Custom,     16, FW_BOLD,   L"Noto Sans Mono Bold");
	create(FontType::Title,      28, FW_BOLD,   L"Noto Sans SC Medium");
	create(FontType::Large,      28, FW_NORMAL, L"Noto Sans TC Medium");
	create(FontType::Bold,       26, FW_BOLD,   L"Perfect DOS VGA 437");
	create(FontType::VeryLarge,  32, FW_BOLD,   L"Perfect DOS VGA 437");
	create(FontType::DmgText,    44, FW_BOLD,   L"Noto Sans JP Medium");
	create(FontType::DeathText,  64, FW_BOLD,   L"Noto Sans JP Medium");
	create(FontType::DeathCount, 48, FW_BOLD,    L"Roboto-Regular");
	return S_OK;
}

ID3DXFont* FontComponent::GetFont(FontType type) const
{
	auto it = fontMap.find(type);
	if (it != fontMap.end())
		return it->second;
	return nullptr;
}

void FontComponent::AddText(const wstring& text, const RECT& rect, Color color, DWORD format, FontType fontType)
{
	TextEntry entry;
	entry.text = text;
	entry.rect = rect;
	entry.format = format;
	entry.color = ToD3DXColor(color);
	entry.fontType = fontType;

	entries.push_back(move(entry));
}

void FontComponent::AddText(const wstring& text, const RECT& rect, D3DXCOLOR color, DWORD format, FontType fontType)
{
	TextEntry entry;
	entry.text = text;
	entry.rect = rect;
	entry.format = format;
	entry.color = color;
	entry.fontType = fontType;

	entries.push_back(move(entry));
}

void FontComponent::Render()
{
	if (!isVisible || entries.empty()) return;

	for (auto& entry : entries)
	{
		ID3DXFont* font = GetFont(entry.fontType);
		if (!font) continue;

		RECT rc = entry.rect;

		if (useTransform)
		{
			auto tf = owner->GetComponent<TransformComponent>();
			if (tf)
			{
				_vec3 pos = tf->GetWorldPosition();     // 이미 스크린 좌표
				LONG w = rc.right  - rc.left;
				LONG h = rc.bottom - rc.top;

			
				if      (pivotFmt & DT_CENTER)  rc.left = (LONG)pos.x - w / 2;
				else if (pivotFmt & DT_RIGHT)   rc.left = (LONG)pos.x - w;
				else                            rc.left = (LONG)pos.x;
				
				if      (pivotFmt & DT_VCENTER) rc.top = (LONG)pos.y - h / 2;
				else if (pivotFmt & DT_BOTTOM)  rc.top = (LONG)pos.y - h;
				else                            rc.top = (LONG)pos.y;

				rc.right = rc.left + w;
				rc.bottom = rc.top + h;
			}
		}
		auto c = entry.color;  c.a *= globalAlpha;
		font->DrawTextW(nullptr, entry.text.c_str(), -1, &rc,
			entry.format, c);
	}
}