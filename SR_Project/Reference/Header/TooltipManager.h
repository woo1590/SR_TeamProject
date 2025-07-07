#pragma once

BEGIN(Engine)

class UIManager;
class SlotComponent;
class FontComponent;

class ENGINE_DLL TooltipManager: public Base
{
public:
	TooltipManager(UIManager* owner);

public:
	void Update(float dt);

	void Show(const ItemInfo& info);
	void Hide();

	void Free() override {}

private:
	UIManager* ui = nullptr;
	FontComponent* font = nullptr;
	bool visible = false;
};

END