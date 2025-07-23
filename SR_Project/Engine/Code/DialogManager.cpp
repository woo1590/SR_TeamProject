#include "EnginePCH.h"
#include "DialogManager.h"
#include "UIManager.h"
#include "FontComponent.h"
#include "PanelComponent.h"
#include "Object.h"
#include "RenderSystem.h"
#include "EngineCore.h"
#include "QuestSystem.h"

void DialogManager::StartDialog(DialogComponent* dialog)
{
	if (!dialog || dialog->IsTalking()) return;

	curDialog = dialog;
	dialog->OnDialogStart();

	EngineCore::GetInstance()->GetRenderSystem()->SetUIRenderState(UIRenderType::QuestUI);

	ShowCurLine();
}

void DialogManager::SkipOrNext()
{
	if (!curDialog || !curDialog->IsTalking()) return;

	if (!isLineFullyDisplayed)
	{
		displayedText = curLine.text;
		isLineFullyDisplayed = true;

		font->ClearText();
		RECT dialongRect = {200, 560, 1200, 700};
		font->AddText(displayedText, dialongRect, Color::White, DT_LEFT | DT_TOP | DT_WORDBREAK, FontType::DeathCount);

		RECT nameRect = {200, 500, 600, 540};
		font->AddText(curDialog->GetSpeakerName(), nameRect,Color::Pink, DT_LEFT | DT_TOP | DT_WORDBREAK, FontType::DeathCount);

		return;
	}
	
	curDialog->AddLineIdx();

	if (curDialog->IsFinished())
		EndDialog();
	else
		ShowCurLine();
}

void DialogManager::ShowCurLine()
{
	if (!panel || !curDialog) return;

	auto owner = panel->GetOwner();
	font = owner->GetComponent<FontComponent>();
	if (!font) return;

	curLine = curDialog->GetCurLine();

	if (onEmotionChange)
		onEmotionChange(curLine.emotion);

	displayedText.clear();
	typingTimer = 0.f;
	isLineFullyDisplayed = false;

	font->ClearText();

	font->SetVisible(true);
	panel->SetVisible(true);
}

void DialogManager::EndDialog()
{
	if (!curDialog) return;

	curDialog->Finish();
	curDialog = nullptr;

	if (onEmotionChange)
		onEmotionChange(Emotion::p6);

	if (panel)
		panel->SetVisible(false);

	if (font)
	{
		font->ClearText();
		font->SetVisible(false);
	}

	EngineCore::GetInstance()->GetRenderSystem()->SetUIRenderState(UIRenderType::MainGame);
	afterDialogTimer = 0.f;
	questInited = true;
}

void DialogManager::Update(float dt)
{
	if (curDialog && curDialog->IsTalking() && !isLineFullyDisplayed && font)
	{
		typingTimer += dt;

		while (typingTimer >= typingSpeed)
		{
			typingTimer -= typingSpeed;

			if (displayedText.length() < curLine.text.length())
			{
				displayedText += curLine.text[displayedText.length()];

				font->ClearText();
				RECT dialogRect = {200, 560, 1100, 700};
				font->AddText(displayedText, dialogRect, Color::White, DT_LEFT | DT_TOP | DT_WORDBREAK, FontType::DeathCount);

				RECT nameRect = {200, 500, 600, 540};
				font->AddText(curDialog->GetSpeakerName(),
					nameRect, Color::Pink, DT_LEFT | DT_TOP | DT_WORDBREAK, FontType::DeathCount);
			}
			else
			{
				isLineFullyDisplayed = true;
				break;
			}
		}
	}
}