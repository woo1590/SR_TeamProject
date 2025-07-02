#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

struct ENGINE_DLL TextEntry
{
	wstring text;
	RECT rect;
	DWORD format = DT_LEFT | DT_TOP;
	D3DXCOLOR color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
};

class ENGINE_DLL FontComponent: public ObjectComponent
{
public:
	explicit FontComponent(Object* owner);
	virtual ~FontComponent();

public:
	static FontComponent* Create(Object* owner);

	void AddText(const wstring& text, const RECT& rect, Color color = Color::White, DWORD format = DT_LEFT | DT_TOP);

	void ClearText() { entries.clear(); }

	void SetFontType(FontType type);
	void Render();
	

private:
	HRESULT CreateFontResource();

private:
	ID3DXFont* font = nullptr;
	
	vector<TextEntry> entries;
};

END