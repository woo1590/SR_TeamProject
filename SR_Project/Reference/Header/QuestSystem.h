#pragma once

BEGIN(Engine)

class UIManager;
class Object;

class ENGINE_DLL QuestSystem : public Base
{
public:
	explicit QuestSystem(UIManager* owner)
		:ui(owner) {
	}

	void Update(float dt);
	void InitQuests();
	void CompleteCurQuest();

	void SetTextObj(Object* obj) { textObj = obj; }
	void SetPanel(Object* _panel) { panel = _panel; }

	void Free() override {};

private:
	void DisplayCurQuest();

private:
	UIManager* ui = nullptr;
	Object* panel = nullptr;
	Object* textObj = nullptr;

	vector<QuestInfo> questList;
	int curQuestIdx = 0;

	wstring curTitle, curDesc;

	bool isCenterPhase = false;
	bool isRightAppearing = false;

	float rightFadeAlpha = 0.f;
	float centerDisplayTime = 3.f;

	const float centerDisplayDuration = 3.f;
	const float fadeDuration = 1.f;
};

END
