#include "EnginePCH.h"
#include "DialogManager.h"
#include "UIManager.h"
#include "FontComponent.h"
#include "PanelComponent.h"
#include "Object.h"
#include "RenderSystem.h"
#include "EngineCore.h"
#include "QuestSystem.h"
#include "SoundManager.h"
#include "EngineCore.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ThirdcamComponent.h"
#include "CameraComponent.h"
#include "Scene.h"

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

	if (isInterrupted && isLineFullyDisplayed)
	{
		isInterrupted = false;
		curLine = interruptedLine;

		displayedText.clear();
		typingTimer = 0.f;
		isLineFullyDisplayed = false;
		soundPlayTimer = minSoundDelay;
		font->ClearText();

		if (onEmotionChange)
			onEmotionChange(curLine.emotion);

		return;
	}

	if (!isLineFullyDisplayed) 
	{
		if (isInterrupted) return;
		consecutiveSkips++;

		if (consecutiveSkips >= skipMaxCount)
		{
			isInterrupted = true;
			interruptedLine = curLine;
			curLine.text = L"제현님! 왜자꾸 대화를 스킵하시는거예요!";
			EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetCameraManager()->GetMainCamera()
				->GetOwner()->GetComponent<ThirdcamComponent>()->SetShake(4.f, 0.5f);
			curLine.emotion = Emotion::p17;
			displayedText.clear();
			typingTimer = 0.f;
			isLineFullyDisplayed = false;
			soundPlayTimer = minSoundDelay;
			font->ClearText();
			consecutiveSkips = 0;

			if (onEmotionChange)
				onEmotionChange(curLine.emotion);

			return;
		}

		displayedText = curLine.text;
		isLineFullyDisplayed = true;

		font->ClearText();
		RECT dialongRect = {200, 560, 1150, 700};
		font->AddText(displayedText, dialongRect, Color::White, DT_LEFT | DT_TOP | DT_WORDBREAK, FontType::DeathCount);

		RECT nameRect = {200, 510, 600, 550};
		font->AddText(curDialog->GetSpeakerName(), nameRect,Color::Pink, DT_LEFT | DT_TOP | DT_WORDBREAK , FontType::DeathCount);

		return;
	}
	else
	{
		//consecutiveSkips = 0;
		curDialog->AddLineIdx();

		if (curDialog->IsFinished())
			EndDialog();
		else
			ShowCurLine();
	}
}

void DialogManager::ShowCurLine()
{
	if (!panel || !curDialog) return;

	auto owner = panel->GetOwner();
	font = owner->GetComponent<FontComponent>();
	if (!font) return;

	curLine = curDialog->GetCurLine();

	if (curLine.onshow)
		curLine.onshow();

	if (onEmotionChange)
		onEmotionChange(curLine.emotion);

	displayedText.clear();
	typingTimer = 0.f;
	isLineFullyDisplayed = false;

	soundPlayTimer = minSoundDelay;

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
	if (!curDialog || !curDialog->IsTalking() || isLineFullyDisplayed || !font) return;

	typingTimer += dt;
	soundPlayTimer += dt;
	
	while (typingTimer >= typingSpeed)
	{
		typingTimer -= typingSpeed;

		if (displayedText.length() < curLine.text.length())
		{
			const wchar_t newChar = curLine.text[displayedText.length()];
			displayedText += newChar;

			if (newChar != L' ' && soundPlayTimer >= minSoundDelay)
			{
				EngineCore::GetInstance()->GetSoundManager()->PlaySFX("switch13");
				soundPlayTimer = 0.f;
			}

			font->ClearText();

			RECT nameRect = {200, 510, 600, 550};
			font->AddText(curDialog->GetSpeakerName(), nameRect, Color::Pink, DT_LEFT | DT_TOP | DT_WORDBREAK, FontType::DeathCount);

			RECT dialogRect = {200, 560, 1150, 700};
			font->AddText(displayedText, dialogRect, Color::White, DT_LEFT | DT_TOP | DT_WORDBREAK , FontType::DeathCount);
		}
		else
		{
			isLineFullyDisplayed = true;
			consecutiveSkips = 0;
			break;
		}
	}
}