#include "EnginePCH.h"
#include "AIController.h"

AIController::AIController(Object* owner)
    :ObjectComponent(owner)
{
}

AIController::~AIController()
{
}

AIController* AIController::Create(Object* owner, BehaviorTree* bt, BlackBoard* bb)
{
    AIController* Instance = new AIController(owner);

    if (FAILED(Instance->Ready_Object(bt, bb)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT AIController::Ready_Object(BehaviorTree* bt, BlackBoard* bb)
{
    SetBehaviorTree(bt);
    SetBlackBoard(bb);

    return S_OK;
}

void AIController::Update(float dt)
{
    if (behaviorTree)
        behaviorTree->Tick(dt, blackBoard);
}

void AIController::SetBehaviorTree(BehaviorTree* bt)
{
    behaviorTree = bt;
}

void AIController::SetBlackBoard(BlackBoard* bb)
{
    blackBoard = bb;
}

BlackBoard* AIController::GetBlackBoard() const
{
    return blackBoard;
}

void AIController::Free()
{
    if (behaviorTree)
        Safe_Release(behaviorTree);

    if (blackBoard)
        Safe_Release(blackBoard);

    ObjectComponent::Free();
}
