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
#include "RenderSystem.h" 

namespace 
{
    constexpr RECT CenterTitle{450, 190, 850, 300}, CenterDesc{450, 250, 850, 450};
    constexpr RECT RightTitle{900, 20, 1280, 70}, RightDesc{900, 70, 1280, 250};
    inline D3DXCOLOR Yellow(float a) { return {1, 1, 0, a}; }
    inline D3DXCOLOR White(float a) { return {1, 1, 1, a}; }
}

void QuestSystem::InitQuests()
{
    quests =
    {
        {L"장비 장착해보기", L"인벤토리에서 장비를 하나 장착해보세요! \n(무기든 방어구든 상관없어요~)",
        QuestType::EquipItem, QuestStatus::NotStarted, 0, 1},

        {L"몬스터 처치", L"근처에 있는 몬스터 3마리 처치하세요", 
        QuestType::KillMonsters, QuestStatus::NotStarted, 0, 3},
        
        {L"마을로 이동",L"이제 마을로 돌아가주세요~! 다들 제현님 기다리고 있어요~",
        QuestType::ReachVillage, QuestStatus::NotStarted, 0, 1}, 
    };
}

void QuestSystem::AcceptQuest(QuestType type)
{
    for (int i = 0; i < (int)quests.size(); ++i)
        if (quests[i].type == type)
            AcceptQuestAtIdx(i);
}

void QuestSystem::AcceptQuestAtIdx(int i)
{
    if (i < 0 || i >= (int)quests.size()) return;
    auto& quest = quests[i];
    if (quest.status != QuestStatus::NotStarted) return;

    quest.status = QuestStatus::InProgress;
    activeIdx = i;
    ChangeState(QuestUIState::CenterFadeOut);
}

QuestStatus QuestSystem::GetStatus(QuestType type) const
{
    for (auto const& quest : quests)
        if (quest.type == type)
            return quest.status;

    return QuestStatus::NotStarted;
}

void QuestSystem::ReportQuestProgress(QuestType type, int amount)
{
    if (activeIdx < 0) return;

    auto& quest = quests[activeIdx];
    if (quest.type == type && quest.status == QuestStatus::InProgress)
    {
        quest.curCount = min(quest.curCount + amount, quest.targetCount);
        if (quest.curCount >= quest.targetCount)
        {
            quest.status = QuestStatus::Completed;
            ChangeState(QuestUIState::RightFadeIn);
        }
    }
}

void QuestSystem::Show(const QuestInfo& q, float alpha, FontType font) 
{
    if (!textObj) return;
    auto* f = textObj->GetComponent<FontComponent>();
    auto* r = textObj->GetComponent<UIRenderer>();
    if (!f || !r) return;

    r->SetVisible(true);
    f->ClearText();

    bool center = (uiPhase.state == QuestUIState::CenterFadeOut);

    wstring tag;
    if (!center)
    {
        if (q.status == QuestStatus::InProgress)  tag = L"(진행중)";
        else if (q.status == QuestStatus::Completed) tag = L"(완료)";
    }

    wstring title = q.title + L" " + tag;
    wstring desc = q.desc;

    if (q.targetCount > 1)
        desc += L" (" + to_wstring(q.curCount) + L"/" + to_wstring(q.targetCount) + L")";

    f->AddText(title, center ? CenterTitle : RightTitle, Yellow(alpha),
        DT_CENTER | DT_WORDBREAK, font);
    f->AddText(desc, center ? CenterDesc : RightDesc, White(alpha),
        DT_CENTER | DT_WORDBREAK, font);
}

void QuestSystem::ChangeState(QuestUIState nextState)
{
    uiPhase = {nextState, 0.f};
}

void QuestSystem::Update(float dt)
{
    if (!textObj || activeIdx < 0 || activeIdx >= quests.size()) return;
    auto& quest = quests[activeIdx];
    uiPhase.t += dt;

    switch (uiPhase.state)
    {
    case QuestUIState::CenterFadeOut:
        Show(quest, clamp(1.f - uiPhase.t / centerDur, 0.f, 1.f), FontType::MineCraftFont);
        if (uiPhase.t >= centerDur)
            ChangeState(QuestUIState::RightFadeIn);
        break;

    case QuestUIState::RightFadeIn:
        Show(quest, clamp(uiPhase.t / fadeDur, 0.f, 1.f), FontType::CookieRunFont);
        if (uiPhase.t >= fadeDur)
            ChangeState(QuestUIState::RightHold);
        break;

    case QuestUIState::RightHold:
        Show(quest, 1.f, FontType::CookieRunFont);
        break;
    }

    const auto& input = EngineCore::GetInstance()->GetInputSystem();
    if (input->IsKeyPressed(KEY::Q))
        ReportQuestProgress(QuestType::KillMonsters, 1);
}