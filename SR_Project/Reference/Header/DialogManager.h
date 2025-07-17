#pragma once

#include "DialogComponent.h"

BEGIN(Engine)

class UIManager;
class FontComponent;
class PanelComponent;

class ENGINE_DLL DialogManager : public Base 
{
public:
	explicit DialogManager(UIManager* owner) : uiMgr(owner) {}

public:
	void StartDialog(DialogComponent* dialog);
	void SkipOrNext();
	void Update(float dt);
	bool IsTalking() const { return curDialog != nullptr && curDialog->IsTalking(); }
	void SetPanel(PanelComponent* _panel) { panel = _panel; }

private:
	void ShowCurLine();
	void EndDialog();
	void Free() override {}

private:
	UIManager* uiMgr = nullptr;
	DialogComponent* curDialog = nullptr;
	FontComponent* font = nullptr;
	PanelComponent* panel = nullptr;

	wstring fullLineText;
	wstring displayedText;
	float typingTimer = 0.f;
	float typingSpeed = 0.05f;
	bool isLineFullyDisplayed = false;

	float afterDialogTimer = 0.f;
	bool questInited = false;
};

END
