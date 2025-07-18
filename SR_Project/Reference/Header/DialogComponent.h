#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

enum class DialogType { Intro, Reminder, AfterQuest1, AfterQuest2 };

struct conditionalDialog
{
	DialogType type;
	vector<wstring> lines;
};

class ENGINE_DLL DialogComponent : public ObjectComponent
{
private:
	DialogComponent(Object* owner) :ObjectComponent(owner) {}

public:
	static DialogComponent* Create(Object* owner) { return new DialogComponent(owner); }

public:
	void SetDialogLines(const vector<DialogLine>& _lines);
	const vector<DialogLine>& GetLines() const { return lines; }

	void OnDialogStart() { talking = true; curLineIdx = 0; }
	void OnDialogEnd() { talking = false; curLineIdx = 0; }

	bool IsTalking() const { return talking; }
	size_t GetCurIdx() const { return curLineIdx; }
	void AddLineIdx();

	const DialogLine& GetCurLine()const;
	bool IsFinished() const { return curLineIdx >= lines.size(); }

	void SetOnFinish(function<void()> cb) { onFinish = move(cb); }
	void Finish() { if (onFinish)onFinish(); OnDialogEnd(); }

	void SetSpeakerName(const wstring& name) { speakerName = name; }
	const wstring& GetSpeakerName() const { return speakerName; }

private:
	vector<DialogLine> lines;
	size_t curLineIdx = 0;
	bool talking = false; 
	wstring speakerName;
	function<void()> onFinish;
};

END