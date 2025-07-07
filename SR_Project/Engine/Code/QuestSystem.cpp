#include "EnginePCH.h"
#include "QuestSystem.h"
#include "UIManager.h"
#include "Object.h"
#include "ObjectManager.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"
#include "Scene.h"
#include "EngineCore.h"
#include "InputSystem.h"

void QuestSystem::InitQuests()
{
	questList.clear();
	curQuestIdx = 0;

	questList.push_back({L"목표 찾기", L"마커를 따라 이동하세요."});
	questList.push_back({L"좀비를 죽이세요", L"근접 무기를 사용하세요."});
	questList.push_back({L"활을 사용하세요", L"스켈레톤을 멀리서 처치하세요."});
	questList.push_back({L"보물 찾기", L"지도를 보고 보물 위치로 가세요."});

	DisplayCurQuest();
}

void QuestSystem::DisplayCurQuest()
{
	if (curQuestIdx >= static_cast<int>(questList.size()) || !textObj) return;

	isCenterPhase = true;
	centerDisplayTime = 0.f;

	auto& quest = questList[curQuestIdx];
	curTitle = quest.title;
	curDesc = quest.desc;

	auto* font = textObj->GetComponent<FontComponent>();
	auto* renderer = textObj->GetComponent<UIRenderer>();
	
	renderer->SetVisible(true);
	font->ClearText();

	RECT titleRect = {425, 290, 800, 360};
	RECT descRect = {410, 350, 900, 500};

	font->AddText(quest.title, titleRect, Color::Yellow, DT_CENTER, FontType::Title);
	font->AddText(quest.desc, descRect, Color::White, DT_CENTER, FontType::QuestTitle);
}

void QuestSystem::CompleteCurQuest()
{
	if (curQuestIdx >= static_cast<int>(questList.size())) return;
	
	questList[curQuestIdx].isCompleted = true;
	++curQuestIdx;

	DisplayCurQuest();
}


void QuestSystem::Update(float dt)
{
	auto input = EngineCore::GetInstance()->GetInputSystem();

	if (input->IsKeyPressed(KEY::Q))
		InitQuests();

	if (input->IsKeyPressed(KEY::E))
		CompleteCurQuest();

	auto* font = textObj->GetComponent<FontComponent>();
	if (!font) return;

	if (isCenterPhase)
	{
		centerDisplayTime += dt;

		float alpha = 1.f - (centerDisplayTime / centerDisplayDuration); 

		alpha = std::clamp(alpha, 0.f, 1.f);

		font->ClearText();

		RECT titleRect = {425, 290, 800, 360};
		RECT descRect = {410, 350, 900, 500};

		D3DXCOLOR titleColor = D3DXCOLOR(1.f, 1.f, 0.f, alpha);
		D3DXCOLOR descColor = D3DXCOLOR(1.f, 1.f, 1.f, alpha);

		font->AddText(curTitle, titleRect, titleColor, DT_CENTER, FontType::Title);
		font->AddText(curDesc, descRect, descColor, DT_CENTER, FontType::QuestTitle);

		if (centerDisplayTime >= centerDisplayDuration)
		{
			isCenterPhase = false;
			isRightAppearing = true;
			rightFadeAlpha = 0.f;
		}
	}
	else if (isRightAppearing)
	{
		rightFadeAlpha += dt / fadeDuration;
		rightFadeAlpha = std::clamp(rightFadeAlpha, 0.f, 1.f);

		font->ClearText();

		RECT titleRect = {1000, 20, 1260, 60};
		RECT descRect = {960, 60, 1260, 110};

		D3DXCOLOR titleColor = D3DXCOLOR(1.f, 1.f, 0.f, rightFadeAlpha);
		D3DXCOLOR descColor = D3DXCOLOR(1.f, 1.f, 1.f, rightFadeAlpha);

		font->AddText(curTitle, titleRect, titleColor, DT_LEFT, FontType::Title);
		font->AddText(curDesc, descRect, descColor, DT_LEFT, FontType::QuestTitle);

		if (rightFadeAlpha >= 1.f)
			isRightAppearing = false;
	}
}
