#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

struct ENGINE_DLL TextEntry
{
	wstring text;
	RECT rect;
	DWORD format = DT_LEFT | DT_TOP;
	D3DXCOLOR color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	FontType fontType = FontType::DeathCount;
};

class ENGINE_DLL FontComponent: public ObjectComponent
{
public:
	explicit FontComponent(Object* owner);

public:
	static FontComponent* Create(Object* owner);
	virtual ~FontComponent();

	void AddText(const wstring& text, const RECT& rect, Color color = Color::White,
		DWORD format = DT_LEFT | DT_TOP, FontType fontType = FontType::Regular);
	void AddText(const wstring& text, const RECT& rect, D3DXCOLOR color,
		DWORD format = DT_LEFT | DT_TOP, FontType fontType = FontType::Regular);

	void Render();
	void ClearText() { entries.clear(); }
	void SetVisible(bool visible) { isVisible = visible; }
	bool IsVisible() const { return isVisible; }
	void SetAlpha(float a) { globalAlpha = clamp(a, 0.f, 1.f); }

	void UseTransform(bool enable = true, DWORD pivot = DT_CENTER | DT_VCENTER)
	{
		useTransform = enable;
		pivotFmt = pivot;
	}

private:
	HRESULT CreateFontResource();
	ID3DXFont* GetFont(FontType type) const;

public:
	unordered_map<FontType, ID3DXFont*> fontMap;
	vector<TextEntry> entries;
	bool isVisible = true;
	float globalAlpha = 1.f;

	bool useTransform = false;
	DWORD pivotFmt = DT_CENTER | DT_VCENTER;
};

END