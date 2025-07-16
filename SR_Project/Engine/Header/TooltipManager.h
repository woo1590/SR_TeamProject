#pragma once

BEGIN(Engine)

class UIManager;
class Object;

struct TooltipData
{
	Object* obj = nullptr;
	float alpha = 0.f;
	bool fadingIn = false;
};

class ENGINE_DLL TooltipManager : public Base
{
public:
	explicit TooltipManager(UIManager* owner) :uiMgr(owner) {}

public:
	void SetWorldTooltip(Object* obj) { worldTip.obj = obj; }
	void SetInventoryTooltip(Object* obj) { invTip.obj = obj; }

	void ShowTooltip(TooltipData& tip, const wstring& text, float x, float y, bool above, FontType fontType);
	void ShowInventoryTooltip(const wstring& text, float x, float y);
	void ShowWorldTooltip(const wstring& text, float x, float y);

	void HideTooltip(TooltipData& tip);
	void HideInventoryTooltip();
	void HideWorldTooltip();

	void Update(float dt);
	void Free() override {}

private:
	UIManager* uiMgr = nullptr;
	
	TooltipData worldTip;
	TooltipData invTip;
};

END