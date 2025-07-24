#pragma once

BEGIN(Engine)
class UIManager;
class Object;

enum class QuestUIState { CenterFadeOut, RightFadeIn, RightHold};
enum class QuestStatus { NotStarted, InProgress, Completed};
enum class QuestType { EquipArmor, KillMonsters, ReachVillage, BuyPig, KillRedGolem, KillEnder,None };

struct QuestInfo
{
    wstring title, desc;
    QuestType type = QuestType::None;
    QuestStatus status = QuestStatus::NotStarted;
    int curCount = 0;
    int targetCount = 0;
};

class ENGINE_DLL QuestSystem : public Base
{
public:
    explicit QuestSystem(UIManager* owner) : uiMgr(owner) {}

    void InitQuests();
    void Update(float dt);

    void ReportQuestProgress(QuestType type, int amount = 1);
    void AcceptQuest(QuestType type);
    QuestStatus GetStatus(QuestType type) const;

    void SetTextObj(Object* obj) { textObj = obj; }

private:
    void Show(const QuestInfo& quest, float alpha, FontType type);
    void ChangeState(QuestUIState nextState);
    void AcceptQuestAtIdx(int i);

    void Free() override {}

private:
    UIManager* uiMgr  = nullptr;
    Object *textObj  = nullptr;
    struct { QuestUIState state; float t; }uiPhase{QuestUIState::RightHold, 0.f};

    static vector<QuestInfo> quests;
    static int activeIdx;
    static int lastCompletedIdx;
    static constexpr float centerDur = 2.f;
    static constexpr float fadeDur = 0.5f;
    static constexpr float holdDur = 3.f;
};
END