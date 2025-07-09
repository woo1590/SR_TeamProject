#pragma once

BEGIN(Engine)

class UIManager;
class Object;

class ENGINE_DLL TooltipManager : public Base
{
public:
	explicit TooltipManager(UIManager* owner) :ui(owner) {}

public:
	void SetWorldTooltip(Object* tooltip) { worldTooltip = tooltip; }
	void ShowWorldTooltip(const wstring& text, float x, float y);
	void HideWorldTooltip();

	void Free() override {}

private:
	UIManager* ui = nullptr;
	Object* worldTooltip = nullptr;
};

END