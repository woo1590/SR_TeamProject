#pragma once
#include "Base.h"
#include "BehaviorTree.h"
#include "BlackBoard.h"

BEGIN(Engine)

class Object;
class ENGINE_DLL AIController :
    public Base
{
protected:
    explicit AIController(Object* owner);
    virtual ~AIController();

public:
    static AIController* Create(Object* owner, BehaviorTree* bt, BlackBoard* bb);

    virtual HRESULT Ready_Object(BehaviorTree* bt, BlackBoard* bb);
    virtual void Tick(float dt);

    void SetBehaviorTree(BehaviorTree* tree);
    void SetBlackBoard(BlackBoard* board);
    BlackBoard* GetBlackBoard() const;

protected:
    void Free() override;

protected:
    BehaviorTree* behaviorTree = nullptr;
    BlackBoard* blackBoard = nullptr;
    Object* Owner = nullptr;
};

END