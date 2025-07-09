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

	void SetInventoryTooltip(Object* tooltip) { inventoryTooltip = tooltip; }
	void ShowInventoryTooltip(const wstring& text, float x, float y);
	void HideInventoryTooltip();

	void Update(float dt);
	void Free() override {}

private:
	UIManager* ui = nullptr;
	Object* worldTooltip = nullptr;
	Object* inventoryTooltip = nullptr;

	float invAlpha = 0.f;
	bool invFadingIn = false;
};

END