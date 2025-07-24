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
	void SetEmotionChangeCallBack(function<void(Emotion)> cb) { onEmotionChange = move(cb); }

private:
	void ShowCurLine();
	void EndDialog();
	void Free() override {}

private:
	UIManager* uiMgr = nullptr;
	DialogComponent* curDialog = nullptr;
	FontComponent* font = nullptr;
	PanelComponent* panel = nullptr;

	DialogLine curLine;
	wstring displayedText;

	vector<wstring> processedLines;

	float typingTimer = 0.f;
	float typingSpeed = 0.03f;
	bool isLineFullyDisplayed = false;

	float afterDialogTimer = 0.f;
	bool questInited = false;

	float soundPlayTimer = 0.f;
	const float minSoundDelay = 0.08f;

	function<void(Emotion)> onEmotionChange;

	int consecutiveSkips = 0;
	bool isInterrupted = false;
	DialogLine interruptedLine;

	int skipMaxCount = 15;
};

END
