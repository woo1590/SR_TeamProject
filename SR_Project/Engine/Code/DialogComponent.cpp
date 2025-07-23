#include "EnginePCH.h"
#include "DialogComponent.h"

void DialogComponent::SetDialogLines(const vector<DialogLine>& _lines)
{
	lines = _lines;
	curLineIdx = 0;
}

const DialogLine& DialogComponent::GetCurLine() const
{
	if (curLineIdx < lines.size())
		return lines[curLineIdx];

	static const DialogLine emptyLine = {L"...", Emotion::p6};
	return emptyLine;
}

void DialogComponent::AddLineIdx()
{
	if (!IsFinished())
		curLineIdx++;
}
