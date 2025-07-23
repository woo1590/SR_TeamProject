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
#include "InventoryComponent.h"
#include "UIManager.h"

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
    {   {L"[소녀의 기도]", L"아리아가 건네준 갑옷을 입어 그녀를 안심시켜 주세요.",
        QuestType::EquipItem, QuestStatus::NotStarted, 0, 1},

        {L"[희망의 증명]", L"아리아를 위협하는 몬스터 3마리를 처치하세요.", 
        QuestType::KillMonsters, QuestStatus::NotStarted, 0, 3},
        
        {L"[쓸쓸한 귀향]",L"아리아와 함께 그녀의 마을로 돌아가세요.",
        QuestType::ReachVillage, QuestStatus::NotStarted, 0, 1}, 
        
        {L"[새로운 가족]",L"아리아가 외롭지 않도록 상점에서 아기 돼지를 구매해 선물하세요.", QuestType::BuyPig,QuestStatus::NotStarted, 0, 1},
        
        {L"[대지를 잠재워줘]",L"'하늘섬'으로 가서 '레드 골렘'을 처치하세요.",
        QuestType::KillRedGolem,QuestStatus::NotStarted, 0, 1},

        {L"[악몽의 종언]",L"밤의 장막 속에 숨어있는 '엔더'를 찾아 처치하세요.",
        QuestType::KillEnder,QuestStatus::NotStarted, 0, 1},
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

void QuestSystem::LoadDataFrom()
{
    auto invComp = uiMgr->GetScene()->GetObjectManager()->GetFrontObject(ObjectType::Player)->GetComponent<InventoryComponent>();
    if (invComp->HasQuestData())
    {
        quests = invComp->GetQuestData();
        for (int i{}; i < quests.size(); ++i)
        {
            if (quests[i].status == QuestStatus::InProgress)
            {
                activeIdx = i;
                break;
            }
        }
    }
}

void QuestSystem::SaveDataTo()
{
    auto invComp = uiMgr->GetScene()->GetObjectManager()->GetFrontObject(ObjectType::Player)->GetComponent<InventoryComponent>();
    invComp->SetQuestData(quests);
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
    {
        ReportQuestProgress(QuestType::EquipItem, 1);
        ReportQuestProgress(QuestType::KillMonsters, 1);
        ReportQuestProgress(QuestType::ReachVillage, 1);
        ReportQuestProgress(QuestType::BuyPig, 1);
        ReportQuestProgress(QuestType::KillRedGolem, 1);
        ReportQuestProgress(QuestType::KillEnder, 1);
    }
}