#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

class ENGINE_DLL FontComponent: public ObjectComponent
{
public:
	explicit FontComponent(Object* owner);
	virtual ~FontComponent();

public:
	static FontComponent* Create(Object* owner);

	void SetText(const wstring& text) { this->text = text; }
	void SetColor(Color color) { this->color = ToD3DXColor(color); }
	void SetFontType(FontType type);

	void SetRect(const RECT& rect) { this->rect = rect; }
	void SetAlign(DWORD format) { this->format = format;}

	void Render();

private:
	HRESULT CreateFontResource();

private:
	ID3DXFont* font = nullptr;
	wstring text;
	D3DXCOLOR color;
	RECT rect = {0, 0, 100, 100};
	DWORD format = DT_LEFT | DT_TOP;
};

END