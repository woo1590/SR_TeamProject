#include "EnginePCH.h"
#include "DialogComponent.h"

void DialogComponent::Finish()
{
	if (!talking) return;

	talking = false;
	curLineIdx = 0;

	if (onFinish)
		onFinish();
}

void DialogComponent::AddLineIdx()
{
	if (curLineIdx + 1 < dialogLines.size())
		++curLineIdx;
	else
		curLineIdx = dialogLines.size();
}